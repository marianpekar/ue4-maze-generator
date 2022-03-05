// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS()
class MAZE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    class MazeData {
    public:
        MazeData(AMazeGenerator& MazeGen) : MazeGen{MazeGen} {}

        void Init() { Data.Init(1, MazeGen.Width * MazeGen.Height); }
        int8 Get(int32 X, int32 Y) const { return Data[X + Y * MazeGen.Width]; }
        void Set(int32 X, int32 Y, int8 value) { Data[X + Y * MazeGen.Width] = value; }
    private:
        AMazeGenerator& MazeGen;
        TArray<int8> Data;
    };
    MazeData Maze = MazeData(*this);

    class WorldDirections {
    public:
        void Shuffle() {
            for (int32 i = Data.Num() - 1; i != 0; i--) {
                int32 j = rand() % i;
                Data.Swap(i, j);
            }
        }
        int8& operator[](size_t i) { return Data[i]; }
        int32 Num() { return Data.Num(); }
    private:
        TArray<int8> Data = { 0, 1, 2, 3 };
    };
    WorldDirections Directions;

	void Step(int32 X, int32 Y);
	void Draw() const;
	void PlacePiece(int32 X, int32 Y, float Yaw, TSubclassOf<AActor> Piece) const;
	bool IsPatternMatching(int32 X, int32 Y, TArray<int8> Pattern) const;

	// Straight
    TArray<int8> HorizontalStraightPattern = { 5, 1, 5,
                                               0, 0, 0,
                                               5, 1, 5 };

    TArray<int8> VerticalStraightPattern = { 5, 0, 5,
                                             1, 0, 1,
                                             5, 0, 5 };

	// T Junctions	
    TArray<int8> TJunctionUpPattern = { 1, 0, 1,
                                        0, 0, 0,
                                        5, 1, 5 };

    TArray<int8> TJunctionDownPattern = { 5, 1, 5,
                                          0, 0, 0,
                                          1, 0, 1 };

    TArray<int8> TJunctionLeftPattern = { 1, 0, 5,
                                          0, 0, 1,
                                          1, 0, 5 };

    TArray<int8> TJunctionRightPattern = { 5, 0, 1,
                                           1, 0, 0,
                                           5, 0, 1 };

	// Crossroad
    TArray<int8> CrossroadPattern = { 1, 0, 1,
                                      0, 0, 0,
                                      1, 0, 1 };

	// Turns
    TArray<int8> TurnLeftUpPattern = { 1, 0, 5,
                                       0, 0, 1,
                                       5, 1, 5 };

    TArray<int8> TurnLeftDownPattern = { 5, 1, 5,
                                         0, 0, 1,
                                         1, 0, 5 };

    TArray<int8> TurnRightUpPattern = { 5, 0, 1,
                                        1, 0, 0,
                                        5, 1, 5 };

    TArray<int8> TurnRightDownPattern = { 5, 1, 5,
                                          1, 0, 0,
                                          5, 0, 1 };

	// Dead ends
    TArray<int8> DeadEndUpPattern = { 5, 0, 5,
                                      1, 0, 1,
                                      5, 1, 5 };

    TArray<int8> DeadEndDownPattern = { 5, 1, 5,
                                        1, 0, 1,
                                        5, 0, 5 };

    TArray<int8> DeadEndLeftPattern = { 5, 1, 5,
                                        0, 0, 1,
                                        5, 1, 5 };

    TArray<int8> DeadEndRightPattern = { 5, 1, 5,
                                         1, 0, 0,
                                         5, 1, 5 };

public:
	UPROPERTY(EditAnywhere)
		int32 StartX = 5;

	UPROPERTY(EditAnywhere)
		int32 StartY = 5;

	UPROPERTY(EditAnywhere)
		int32 Width = 100;

    UPROPERTY(EditAnywhere)
        int32 Height = 50;

	UPROPERTY(EditAnywhere)
		int32 Scale = 300;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> StraightPiece;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> TJunctionPiece;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> CrossroadPiece;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> TurnPiece;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> DeadEndPiece;

};
