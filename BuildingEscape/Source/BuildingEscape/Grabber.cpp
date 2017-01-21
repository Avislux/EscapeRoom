// Copyright Ben Tristem 2016.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty."));
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	FVector LineTraceEnd = GetReachLineEnd();
	if (!PhysicsHandle) { return; }
	//if physics handle attached, move object being held
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle found! %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No InputComponent found! %s"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() //Line trace for grabber
{
	
	/*
	DrawDebugLine(GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0), false, 0.f, 0.f, 10.f);*/

	///setup query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); //ignore owner otherwise it would detect owner as first hit

																			   /// ray cast /line tracer out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(), 
		GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	
	//see what we hit
	AActor* ActorHit = Hit.GetActor();
	//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(ActorHit->A));
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(ActorHit->GetName()));
	}
	//UE_LOG(LogTemp, Warning, TEXT("Location:%s, Position: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
	return Hit;
}
void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	}
}
void UGrabber::Release() {
	//if (!PhysicsHandle) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return LineTraceEnd;
}