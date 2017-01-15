// Copyright Ben Tristem 2016.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	Owner = GetOwner();
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	
}

void UOpenDoor::OpenDoor() {

	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}
void UOpenDoor::CloseDoor() {

	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}
// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll trigger volume every frame
	if (GetTotalMassOFActorsOnPlate() > 50.f) {

		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOFActorsOnPlate() 
{
	float totalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors); //Initializes OverlappingActors
	for (auto& actor : OverlappingActors)
	{
		float mass = actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		totalMass += mass;
		UE_LOG(LogTemp, Warning, TEXT("OverlappingActors: %s, with mass "), *actor->GetName());
	}
	return totalMass;
}