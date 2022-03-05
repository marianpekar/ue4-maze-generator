// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"
#include "Math/Vector.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
    Super::BeginPlay();
    
    Maze.Init();
    Step(StartX, StartY);

    Draw();
}

void AMazeGenerator::Step(int32 X, int32 Y)
{
    Directions.Shuffle();

    for (size_t i = 0; i < 4; i++) {
        switch (Directions[i])
        {
        case 0: // Up
            if (X + 2 >= Width - 1 || Maze.Get(X + 2, Y) == 0)
                continue;

            Maze.Set(X + 2, Y, 0);
            Maze.Set(X + 1, Y, 0);
            Step(X + 2, Y);

        case 1: // Right
            if (Y - 2 <= 0 || Maze.Get(X, Y - 2) == 0)
                continue;

            Maze.Set(X, Y - 2, 0);
            Maze.Set(X, Y - 1, 0);
            Step(X, Y - 2);

        case 2: // Down
            if (X - 2 <= 0 || Maze.Get(X - 2, Y) == 0)
                continue;

            Maze.Set(X - 2, Y, 0);
            Maze.Set(X - 1, Y, 0);
            Step(X - 2, Y);

        case 3: // Left
            if (Y + 2 >= Height - 1 || Maze.Get(X, Y + 2) == 0)
                continue;

            Maze.Set(X, Y + 2, 0);
            Maze.Set(X, Y + 1, 0);
            Step(X, Y + 2);
        }
    }
}

void AMazeGenerator::Draw() const
{
    for (int32 x = 1; x < Width - 1; x++)
    {
        for (int32 y = 1; y < Height - 1; y++)
        {
            // Straight
            if (IsPatternMatching(x, y, HorizontalStraightPattern)) { PlacePiece(x, y, 90.f, StraightPiece); }
            if (IsPatternMatching(x, y, VerticalStraightPattern))   { PlacePiece(x, y,  0.f, StraightPiece); }

            // T Junctions	
            if (IsPatternMatching(x, y, TJunctionUpPattern))    { PlacePiece(x, y, -90.f, TJunctionPiece); }
            if (IsPatternMatching(x, y, TJunctionDownPattern))  { PlacePiece(x, y,  90.f, TJunctionPiece); }
            if (IsPatternMatching(x, y, TJunctionLeftPattern))  { PlacePiece(x, y,   0.f, TJunctionPiece); }
            if (IsPatternMatching(x, y, TJunctionRightPattern)) { PlacePiece(x, y, 180.f, TJunctionPiece); }

            // Crossroad
            if (IsPatternMatching(x, y, CrossroadPattern)) { PlacePiece(x, y, 0.f, CrossroadPiece); }

            // Turns
            if (IsPatternMatching(x, y, TurnLeftUpPattern))    { PlacePiece(x, y,   0.f, TurnPiece); }
            if (IsPatternMatching(x, y, TurnLeftDownPattern))  { PlacePiece(x, y,  90.f, TurnPiece); }
            if (IsPatternMatching(x, y, TurnRightUpPattern))   { PlacePiece(x, y, -90.f, TurnPiece); }
            if (IsPatternMatching(x, y, TurnRightDownPattern)) { PlacePiece(x, y, 180.f, TurnPiece); }

            // Dead ends
            if (IsPatternMatching(x, y, DeadEndUpPattern))    { PlacePiece(x, y,  90.f, DeadEndPiece); }
            if (IsPatternMatching(x, y, DeadEndDownPattern))  { PlacePiece(x, y, -90.f, DeadEndPiece); }
            if (IsPatternMatching(x, y, DeadEndLeftPattern))  { PlacePiece(x, y, 180.f, DeadEndPiece); }
            if (IsPatternMatching(x, y, DeadEndRightPattern)) { PlacePiece(x, y,   0.f, DeadEndPiece); }
        }
    }
}

void AMazeGenerator::PlacePiece(int32 X, int32 Y, float Yaw, TSubclassOf<AActor> Piece) const
{
    FVector Location(X * Scale, Y * Scale, 0);
    FRotator Rotation(0, Yaw, 0);
    FActorSpawnParameters SpawnInfo;

    GetWorld()->SpawnActor<AActor>(Piece, Location, Rotation, SpawnInfo);
}

// All patterns are defined in Maze.h
// Example pattern: [ 1 0 1 
//                    0 0 0 
//                    1 0 1 ] is a crossroad.
bool AMazeGenerator::IsPatternMatching(int32 X, int32 Y, TArray<int8> Pattern) const
{
    int Count = 0;
    int i = 0;
    for (int y = 1; y > -2; y--)
    {
        for (int x = -1; x < 2; x++)
        {
            if (Pattern[i] == Maze.Get(X + x, Y + y) || Pattern[i] == 5)
            {
                Count++;
            }

            i++;
        }
    }

    return Count == 9;
}