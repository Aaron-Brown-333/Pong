#include "PongGameState.h"

void APongGameState::IncrementPlayerScore(int player)
{
	if (player == 1) {
		Player1Score++;
	}
	else if (player == 2) {
		Player2Score++;
	}
}

void APongGameState::ResetPlayerScores()
{
	Player1Score = 0;
	Player2Score = 0;
}

void APongGameState::ResetPredictedExitPoint()
{
	PredictedExitPoint = FVector::ZeroVector;
}