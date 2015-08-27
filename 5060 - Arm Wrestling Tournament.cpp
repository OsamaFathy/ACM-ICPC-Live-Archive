// Handle: Osama.F
// Problem: 5060 - Arm Wrestling Tournament
// Submission ID: 1771178
// Verdict: AC
// Runtime: 0.053

// Hint: Segment tree concept

#include <iostream>
#include <vector>
using namespace std;

const int kMaxPlayers = (1 << 15);

struct Position {
    int winner;
	int player2;
	int winners_strength;
	int winner_prev_match;
	Position() {
	}
	Position(int p1, int p2, int winst, int prev) :
			winner(p1), player2(p2), winners_strength(winst), winner_prev_match(
					prev) {
	}
};

int num_players;
int recovery_strength;
int original_strengths[kMaxPlayers];
Position seg_array[kMaxPlayers * 2 + 1];

Position Merge_Positions(const Position &p1, const Position &p2) {
	Position merged_pos;
	if (p1.winners_strength >= p2.winners_strength) {
		merged_pos.winner = p1.winner;
		merged_pos.player2 = p2.winner;
		merged_pos.winners_strength = min(original_strengths[p1.winner],
				p1.winners_strength - p2.winners_strength + recovery_strength);
	} else {
		merged_pos.winner = p2.winner;
		merged_pos.player2 = p1.winner;
		merged_pos.winners_strength = min(original_strengths[p2.winner],
				p2.winners_strength - p1.winners_strength + recovery_strength);
	}
	merged_pos.winners_strength = max(merged_pos.winners_strength, 0);
	return merged_pos;
}
void Build_Seg_Array(int pos, int low, int hi) {
	if (low == hi) {
		seg_array[pos] = Position(low, -1, original_strengths[low], -1);
		return;
	}

	int mid = (low + hi) / 2;
	Build_Seg_Array(pos * 2, low, mid);
	Build_Seg_Array(pos * 2 + 1, mid + 1, hi);
	seg_array[pos] = Merge_Positions(seg_array[pos * 2],
			seg_array[pos * 2 + 1]);
	if (seg_array[pos].winner == seg_array[pos * 2].winner)
		seg_array[pos].winner_prev_match = pos * 2;
	else
		seg_array[pos].winner_prev_match = pos * 2 + 1;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	int T;
	cin >> T;
	while (T--) {
		int n;
		cin >> n >> recovery_strength;
		num_players = (1 << n);
		for (int i = 0; i < num_players; i++)
			cin >> original_strengths[i];
		Build_Seg_Array(1, 0, num_players - 1);

		cout << seg_array[1].winner + 1 << endl;
		vector<int> played_with(n);
		int cur_pos = 1;
		for (int i = 0; i < n; i++) {
			played_with[i] = seg_array[cur_pos].player2;
			cur_pos = seg_array[cur_pos].winner_prev_match;
		}
		for (int i = n - 1; i >= 0; i--)
			cout << played_with[i] + 1 << (i != 0 ? " " : "\n");
	}
	return 0;
}
