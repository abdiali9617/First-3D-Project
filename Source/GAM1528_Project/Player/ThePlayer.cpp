// Fill out your copyright notice in the Description page of Project Settings.

#include "ThePlayer.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "LevelMechanics/Interactables.h"
#include "LevelMechanics/ExitDoor.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationTypes.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/PrimitiveComponent.h"
#include "GunComponent/GunComponent.h"
#include "GunComponent/RifleGunComponent.h"
#include "GunComponent/PistolGunComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"
#include "Components/PointLightComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Monster/Monsters.h"
#include "Game/MyGameState.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Components/LightComponent.h"
#include "Components/DirectionalLightComponent.h"

// Sets default values
AThePlayer::AThePlayer() 
   /* PistolGun(nullptr),
    RifleGun(nullptr),
    ActiveGun(nullptr)*/
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set the collision on the player 
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = GetCapsuleComponent();

    // Setting Up Capsule to Apply Damage
    DamageCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageCapsule"));
    DamageCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DamageCapsule->SetCollisionProfileName("OverlapAll");
    DamageCapsule->OnComponentBeginOverlap.AddDynamic(this, &AThePlayer::HandleOverlap);
    DamageCapsule->SetupAttachment(RootComponent);

    // Creating the Camera
    FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
    FPSCamera->SetupAttachment(RootComponent);
    FPSCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
    FPSCamera->bUsePawnControlRotation = true;

    // Setting the player's model to not be seen in game by the player
    GetMesh()->SetOwnerNoSee(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //GetMesh()->GetBodyInstance()->bLockXRotation = true;
    //GetMesh()->GetBodyInstance()->bLockYRotation = true;
    GetMesh()->SetupAttachment(RootComponent);

    // Mesh For Arms Holding the Rifle
    FPSArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSArmsMesh"));
    FPSArmsMesh->SetOnlyOwnerSee(true);
    FPSArmsMesh->SetVisibility(true);
    FPSArmsMesh->SetupAttachment(FPSCamera);
    FPSArmsMesh->bCastDynamicShadow = false;
    FPSArmsMesh->CastShadow = false;

    // Creating the Mesh for the pistol 
    FPSPistolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSPistolMesh"));
    FPSPistolMesh->SetOnlyOwnerSee(true);
    FPSPistolMesh->SetVisibility(false);
    FPSPistolMesh->SetupAttachment(FPSCamera);
    FPSPistolMesh->bCastDynamicShadow = false;
    FPSPistolMesh->CastShadow = false;

    // Creating the Mesh for the rifle 
    FPSRifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSRifleMesh"));
    FPSRifleMesh->SetOnlyOwnerSee(true);
    FPSRifleMesh->SetVisibility(true);
    FPSRifleMesh->SetupAttachment(FPSCamera);
    FPSRifleMesh->bCastDynamicShadow = false;
    FPSRifleMesh->CastShadow = false;

    // Creating the GunComponent for the Pistol
    PistolGun = CreateDefaultSubobject<UPistolGunComponent>(TEXT("Pistol"));
    PistolGun->InitializeComponent();

    // Creating the GunComponent for the Rifle
    RifleGun = CreateDefaultSubobject<URifleGunComponent>(TEXT("Rilfe"));
    RifleGun->InitializeComponent();

    // Creating the starting point of the Guns
    PistolSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PistolSpawnBox"));
    PistolSpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PistolSpawnBox->SetCollisionProfileName("NoCollision");
    PistolSpawnBox->SetupAttachment(FPSPistolMesh);

    RifleSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RifleSpawnBox"));
    RifleSpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RifleSpawnBox->SetCollisionProfileName("NoCollision");
    RifleSpawnBox->SetupAttachment(FPSRifleMesh);

    // Initializing the Location of the Movement Audio
    MovementSoundLocation = CreateDefaultSubobject<UBoxComponent>(TEXT("MovementSoundLocation"));
    MovementSoundLocation->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MovementSoundLocation->SetCollisionProfileName("NoCollision");
    MovementSoundLocation->SetupAttachment(RootComponent);

    LaserSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserSpawn"));
    LaserSpawn->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LaserSpawn->SetCollisionProfileName("NoCollision");
    LaserSpawn->SetupAttachment(FPSRifleMesh);
   
    LaserBeam = CreateDefaultSubobject<UParticleSystemComponent>("LaserBeam");
    // Initializing the Audio for the Guns
    RifleAudio = CreateDefaultSubobject<UAudioComponent>("RifleAudio");
    RifleAudio->bAutoActivate = false;
    RifleAudio->SetupAttachment(RifleSpawnBox);

    PistolAudio = CreateDefaultSubobject<UAudioComponent>("PistolAudio");
    PistolAudio->bAutoActivate = false;
    PistolAudio->SetupAttachment(PistolSpawnBox);

    RifleReloadAudio = CreateDefaultSubobject<UAudioComponent>("RifleReloadAudio");
    RifleReloadAudio->bAutoActivate = false;
    RifleReloadAudio->SetupAttachment(RifleSpawnBox);

    PistolReloadAudio = CreateDefaultSubobject<UAudioComponent>("PistolReloadAudio");
    PistolReloadAudio->bAutoActivate = false;
    PistolReloadAudio->SetupAttachment(PistolSpawnBox);

    // Initializing the Audio for the Movement
    WalkAudio = CreateDefaultSubobject<UAudioComponent>("WalkAudio");
    WalkAudio->bAutoActivate = false;
    WalkAudio->SetupAttachment(MovementSoundLocation);

    SprintAudio = CreateDefaultSubobject<UAudioComponent>("SprintAudio");
    SprintAudio->bAutoActivate = false;
    SprintAudio->SetupAttachment(MovementSoundLocation);

    ActiveMovementAudio = CreateDefaultSubobject<UAudioComponent>("ActiveMovementAudio");
    ActiveMovementAudio->bAutoActivate = false;
    ActiveMovementAudio->SetupAttachment(MovementSoundLocation);

    // Initializing and setting defaults for memebr variables
    PlayerHealthMax = 100;
    NumOfMedPacks = 2;
    MaxWalkSpeed = 500;
    SprintWalkSpeed = 2000;
    RegularWalkSpeed = 500;
    MaxStamina = 100;
    AimisActive = false;
    IsMovingRight = false;
    IsMovingFoward = false;
    bHasBlackBox = false;

    SetReplicates(true);
    SetReplicateMovement(true);
}

void AThePlayer::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AThePlayer::BeginPlay()
{
    Super::BeginPlay();
    //  // Initializing the Starting Values of different Variables
    CurrentStamina = MaxStamina;
    PlayerHealth = PlayerHealthMax;
    NumOfMedPacks = 2;
    ActiveGun = RifleGun;
    m_Trackers = 10;
    NetSetPlayerModelOff();
    StartPos = GetActorLocation();
}

// Called every frame
void AThePlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Updating the Movement Speed
    Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = MaxWalkSpeed;
    // Replenishing the Stamia
    ServerReplenishStamina();
    // Networking the Rotation
    FPSCamera->SetWorldRotation(GetViewRotation());
    // Clamping the Rotation 
    FMath::Clamp(Cast<APlayerController>(GetController())->RotationInput.Pitch, 0.0f, 0.0f);
    // Updating the Camera's field of view 
    SetAiming(DeltaTime);
    // Updating The Visible Weapon
    ClientVisibleWeapon();
    // Playing the Proper Movement Audio
    SetMovementAudio();
    // Shows the Laser if the rifle is active
    NetLaser();
}

void AThePlayer::SetAiming(float DeltaTime)
{
    // if the player is trying to aim and the field of view isn't at the max
    if (AimisActive == false && FPSCamera->FieldOfView < 90.0f)
    {
        //Interpolate between the two values  until it reaches the right point
        FPSCamera->SetFieldOfView(FMath::FInterpConstantTo(NotActive, 90.0f, DeltaTime, 1.0f));
        NotActive += 3;
        if (FPSCamera->FieldOfView == 90.0f)
            NotActive = 45.0f;
    }
    // if the player isn't trying to aim and the field of view isn't at the min
    if (AimisActive == true && FPSCamera->FieldOfView > 45.0f)
    {
        //Interpolate between the two values  until it reaches the right point
        FPSCamera->SetFieldOfView(FMath::FInterpConstantTo(Active, 45.0f, DeltaTime, 1.0f));
        Active -= 3;
        if (FPSCamera->FieldOfView == 45.0f)
            Active = 90.0f;
    }
}

// Called to bind functionality to input
void AThePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &AThePlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AThePlayer::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AThePlayer::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AThePlayer::AddControllerPitchInput);

    //Movement
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AThePlayer::OnStartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AThePlayer::OnStopJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AThePlayer::ServerStartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AThePlayer::ServerStopSprint);

    //gunplay
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AThePlayer::ServerFire);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AThePlayer::ServerReloadCurrentWeapon);
    PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &AThePlayer::ServerSwitchWeapon);
    PlayerInputComponent->BindAction("MedPack", IE_Pressed, this, &AThePlayer::ServerUseMedPack);
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AThePlayer::AimWeapon);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &AThePlayer::StopAimWeapon);

    // Player Revival
    PlayerInputComponent->BindAction("Revival", IE_Pressed, this, &AThePlayer::ServerRevival);

    //MISC.
    PlayerInputComponent->BindAction("Mark", IE_Pressed, this, &AThePlayer::MarkLocation);
}

void AThePlayer::MoveForward(float Val)
{
    // Move the Player in the proper direction when the input is pressed 
    if (Val != 0)
    {
        IsMovingFoward = true;
        // Set the Proper Speed
        ServerSetSprintSpeed(Val);
        FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
        AddMovementInput(Direction, Val);
    }
    //if the player isn't moving 
    else
        IsMovingFoward = false;
}

void AThePlayer::MoveRight(float Val)
{
    // Move the Player in the proper direction when the input is pressed 
    if (Val != 0)
    {
        IsMovingRight = true;
        FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
        AddMovementInput(Direction, Val);
    }
    //if the pl;ayer isn't moving 
    else
        IsMovingRight = false;
}

void AThePlayer::SetMovementAudio()
{
    // The  Player has to be sprinting , moving foward and not moving Right to play Sprint Audio
    if (IsMovingFoward == true && CurrentStamina > 1 && IsMovingRight == false)
    {
        if (bIsSprinting == true)
        {
            if (ActiveMovementAudio != SprintAudio)
            {
                NetStopAudio(ActiveMovementAudio);
                ActiveMovementAudio = SprintAudio;
            }
        }
    }
    // if the player isn't sprinting but moving foward or is Moving Right or Stamina is 1 or less play the walk audio
    if (IsMovingFoward == true && bIsSprinting == false || IsMovingRight == true || CurrentStamina <= 1)
    {
        if (ActiveMovementAudio != WalkAudio)
        {
            NetStopAudio(ActiveMovementAudio);
            ActiveMovementAudio = WalkAudio;
        }
    }
    // if not moving don't play anything
    if (IsMovingFoward == false && IsMovingRight == false)
    {
        NetStopAudio(ActiveMovementAudio);
    }
    //play the active audio
    if (ActiveMovementAudio->IsPlaying() == false)
    {
        NetPlayAudio(ActiveMovementAudio);
    }
}

void AThePlayer::OnStartJump()
{
    // Player can only jump if there is at least 25 stamina left
    if (CurrentStamina > 0)
    {
        bPressedJump = true;
        ServerSubtractStamina(true);
    }
}

void AThePlayer::OnStopJump()
{
    bPressedJump = false;
}

int AThePlayer::getHealth()
{
    return PlayerHealth;
}

int AThePlayer::getHealthPercentage()
{
    // Return the percentage of Health Left
    return (PlayerHealth / PlayerHealthMax) * 100;
}


void AThePlayer::MarkLocation()
{
    // If there are Markers Left Spawn a Flare
    if (m_Trackers >= 1)
    {
        FVector SpawnLoc = GetActorLocation() += FVector(10.0f, 0.0f, 00.0f);
        AActor* NewSpawn = GetWorld()->SpawnActor<AActor>(MarksToSpawn, SpawnLoc, FRotator::ZeroRotator);
        m_Trackers--;
    }
}

void AThePlayer::AimWeapon()
{
    // Set AimActive to True
    AimisActive = true;
}

void AThePlayer::StopAimWeapon()
{
    // Set AimActive to false
    AimisActive = false;
}

void AThePlayer::ApplyPlayerDamage(AActor * OtherActor)
{
    // Damage the Player Based Of the Damage amount of the Monster that hit the player
    AMonsters* monster = Cast<AMonsters>(OtherActor);
    if (monster == nullptr)
        return;
    if (Cast<AMonsters>(OtherActor))
    {
        // if the player is hit by a monster apply damage to the player
        PlayerHealth -= Cast<AMonsters>(OtherActor)->GetDamageDealt();
        if (PlayerHealth <= 0)
            PlayerHealth = 0;
    }
    // Call OnDeath if player's Health is 0
    if (PlayerHealth == 0)
    {
        OnDeath();
    }
}

bool AThePlayer::IsRifleActive()
{
    // If the Rifle is Active return true
    if (ActiveGun == RifleGun)
        return true;
    else
        return false;
}

FRotator AThePlayer::GetViewRotation() const
{
    if (Controller)
    {
        return Controller->GetControlRotation();
    }
    return FRotator(RemoteViewPitch/255.f * 360.f, GetActorRotation().Yaw, 0.f);
}

void AThePlayer::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherActor != nullptr)
    {
        //if the other actor isnt null and has the tag pickup
        if (OtherActor->ActorHasTag("Pickup"))
        {
            //cas it as an interactable
            AInteractables* BlackBox = Cast<AInteractables>(OtherActor);
            //set the bool has black box to true
            bHasBlackBox = true;
            //call the casted actor's IsTaken function
            BlackBox->IsTaken();
        }
        //if its tag is instead door
        if (OtherActor->ActorHasTag("Door"))
        {
            //and the has black box bool is true
            if (bHasBlackBox == true)
            {
                //quit the game after stopping the controller
                APlayerController* Controller = Cast<APlayerController>(GetController());
                UKismetSystemLibrary::QuitGame(GetWorld(), Controller, EQuitPreference::Quit, true);
            }
        }
    }
}

void AThePlayer::ClientEnableInput_Implementation()
{ 
    // Enable Player Input
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    this->EnableInput(PlayerController);
}

void AThePlayer::ClientDisableInput_Implementation()
{
    // Disable Player Input
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    this->DisableInput(PlayerController);
}

void AThePlayer::ServerUseMedPackForRevival_Implementation()
{
    // Subtract NumPack
    NumOfMedPacks--;
}
void AThePlayer::SetHealth_Implementation(int health)
{
    // Set the Player Health
    PlayerHealth = health;
}

void AThePlayer::NetStopAudio_Implementation(UAudioComponent* AudioComponent)
{
    // Stop playing Audio
    AudioComponent->Stop();
}

void AThePlayer::NetPlayAudio_Implementation(UAudioComponent* AudioComponent)
{
    // Start playing Audio
    AudioComponent->Play();
}

void AThePlayer::ServerSetSprintSpeed_Implementation(float Val)
{
    // player cannot sprint if there is no stamina 
    if (Val == 1 && bIsSprinting == true && CurrentStamina > 0)
    {
        // set the sprint speed and subtract stamina
        //bsSprintCheck = true;
        MaxWalkSpeed = SprintWalkSpeed;
        ServerSubtractStamina(false);
    }
    else
    {
        // Set the speed to regular speed
        //bsSprintCheck = false;
        MaxWalkSpeed = RegularWalkSpeed;
    }
}

void AThePlayer::ServerSubtractStamina_Implementation(bool jump)
{
    // If player is Jumping subtract 25 from stamina
    if (jump)
    {
        CurrentStamina -= 25;
        if (CurrentStamina <= 0)
            CurrentStamina = 0;
    }
    // else subtract 0.1 from stamina
    else if (jump == false)
    {
        CurrentStamina -= 0.1;
        if (CurrentStamina <= 0)
            CurrentStamina = 0;
    }
}

void AThePlayer::ServerReplenishStamina_Implementation()
{
    //Replenishes stamina by 0.1 every update
    if (CurrentStamina <= MaxStamina && bIsSprinting == false)
        CurrentStamina = CurrentStamina + 0.1;
}

void AThePlayer::ServerStartSprint_Implementation()
{
    // start sprinting
    bIsSprinting = true;
}

void AThePlayer::ServerStopSprint_Implementation()
{
    // stop sprinting 
    bIsSprinting = false;
}

void AThePlayer::NetSetPlayerModelOff_Implementation()
{
    // Set the mesh to not be seen by the player 
    this->GetMesh()->SetOwnerNoSee(true);
}

// Implementation Functions
void AThePlayer::ServerFire_Implementation()
{
    // The player cannot fire if sprinting
    if (RifleReloadAudio->IsPlaying() == false && PistolReloadAudio->IsPlaying() == false)
    {
        if (bIsSprinting == false)
        {
            // Cannot fire if ammo is 0 on the activegun
            if (ActiveGun->GetCurrentAmmo() != 0)
            {
                // Passing the the info need into the fire function on the active gun
                FHitResult OutHit;
                FVector Start = FVector(0, 0, 0);
                if (ActiveGun == RifleGun)
                    Start = RifleSpawnBox->GetComponentLocation();
                if (ActiveGun == PistolGun)
                    Start = PistolSpawnBox->GetComponentLocation();
                FVector ForwardVector = FPSCamera->GetForwardVector();
                FVector End = ((ForwardVector * 100000.0f) + Start);
                FCollisionQueryParams CollisionParams;
                CollisionParams.AddIgnoredActor(this);

                ActiveGun->FireGun(this, OutHit, Start, ForwardVector, End, CollisionParams);
                // Subtracts the ammo
                ActiveGun->NetSubtractAmmo();
                if (ActiveGun == RifleGun)
                    NetPlayAudio(RifleAudio);

                else if (ActiveGun == PistolGun)
                    NetPlayAudio(PistolAudio);

                // Apply Recoil
                NetRecoil();
                // Plays particle at the Start location
                if (ActiveGun == RifleGun)
                    NetPlayParticleEffect(RifleMuzzleFlash, (ForwardVector * 30.0f) + Start);
                if (ActiveGun == PistolGun)
                    NetPlayParticleEffect(PistolMuzzleFlash, (ForwardVector * 10.0f) + Start);
                // Plays particle at the End location
                NetPlayParticleEffect(BulletImpact, End);
                // if the player hit's a monster apply damage to it 
                if (Cast<AMonsters>(OutHit.GetActor()))
                {
                    AMonsters* monster = (Cast<AMonsters>(OutHit.GetActor()));
                    ServerApplyMonsterDamage(monster, OutHit.GetComponent());
                }
            }
        }
    }
}
void AThePlayer::NetLaser_Implementation()
{
    // Rifle is Acitve
    if (ActiveGun == RifleGun)
    {
        FHitResult OutHit;
        FVector Start = LaserSpawn->GetComponentLocation();
        FVector ForwardVector = FPSCamera->GetForwardVector();
        FVector End = ((ForwardVector * 100000.0f) + Start);
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredComponent(this->GetCapsuleComponent());

        if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
        {

            if (OutHit.GetActor() != nullptr || OutHit.GetComponent() != nullptr)
            {
                End = OutHit.Location;

            }
            else
            {
                End = ((ForwardVector * 100000.0f) + Start);
            }
            //DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, -1, 0, 1.f);
        }
        // Spawn a particle Effect along the raycast line
        LaserBeam->Activate();
        LaserBeam->SetVisibility(true);
        LaserBeam->SetBeamSourcePoint(0, Start, 0);
        LaserBeam->SetBeamTargetPoint(0, End, 0);
    }
    //Set it to off if pistol is active
    else if (ActiveGun == PistolGun)
    {
        LaserBeam->Deactivate();
        LaserBeam->SetVisibility(false);
        LaserBeam->SetBeamSourcePoint(0, FVector(0, 0, 0), 0);
        LaserBeam->SetBeamTargetPoint(0, FVector(0, 0, 0), 0);
    }
}
void AThePlayer::ServerReloadCurrentWeapon_Implementation()
{
    // Calls reload on the active gun
    if (bIsSprinting == false)
    {
        ActiveGun->NetReload();
        if (ActiveGun == RifleGun)
            NetPlayAudio(RifleReloadAudio);
        else if (ActiveGun == PistolGun)
            NetPlayAudio(PistolReloadAudio);
    }
}

void AThePlayer::NetRecoil_Implementation()
{
    // Adds recoil effect to the camera when the gun fires 
    AddControllerPitchInput(FMath::RandRange(-1.0f, -0.5f));
    AddControllerYawInput(FMath::RandRange(-0.5f, 0.5f));
}

void AThePlayer::ClientVisibleWeapon_Implementation()
{
    // If Rifle is Active Set it to Visible
    if (ActiveGun == RifleGun)
    {
        FPSRifleMesh->SetVisibility(true);
        FPSArmsMesh->SetVisibility(true);
        FPSPistolMesh->SetVisibility(false);
    }
    // If Pistol is Active Set it to Visible
    if (ActiveGun == PistolGun)
    {
        FPSPistolMesh->SetVisibility(true);
        FPSArmsMesh->SetVisibility(false);
        FPSRifleMesh->SetVisibility(false);
    }
}

void AThePlayer::ServerSwitchWeapon_Implementation()
{
    // Switches the Gun that is Active
    if (bIsSprinting == false)
    {
        if (ActiveGun)
        {
            // Set's the Pistol's mesh on and the active gun to pistol
            if (ActiveGun == RifleGun)
            {
                ActiveGun = PistolGun;
            }
            // Set's the Rifle's mesh on and the active gun to rifle
            else if (ActiveGun == PistolGun)
            {
                ActiveGun = RifleGun;
            }
        }
    }
}

void AThePlayer::ServerApplyMonsterDamage_Implementation(AMonsters* monster, UPrimitiveComponent* HitComp)
{
    // Sets damage to the Monster that hit using the damage amount from the gun that's active
    int mHealth = monster->GetHealth();
    int damage = ActiveGun->GetDamageDealt();
    int newHealth = 0;
    // If the player hits the Body Apply Regular damage
    if (HitComp->ComponentHasTag("Body"))
        newHealth = mHealth - damage;
    // If the Player Hits 
    else if (HitComp->ComponentHasTag("Head"))
        newHealth = mHealth - (damage * 2);

    if (newHealth <= 0)
    {
        newHealth = 0;
    }
    monster->SetHealth(newHealth);
    if (monster->GetHealth() == 0)
    {
        // if the monster's health is 0 destroy it 
        monster->Destroy();
    }
}

void AThePlayer::ServerApplyPlayerDamage_Implementation(AActor * OtherActor)
{
    // Apply Damage to Player Every 3 Seconds 
    if (GetWorld()->GetTimerManager().IsTimerActive(DamageTimer) == false)
    {
        ApplyPlayerDamage(OtherActor);
        GetWorld()->GetTimerManager().SetTimer(DamageTimer, 2.0f, false);
    }
}

void AThePlayer::ServerUseMedPack_Implementation()
{
    // Recovers the Player's Health if there are MedPacks left
    if (NumOfMedPacks != 0)
    {
        if (PlayerHealth > 0)
        {
            PlayerHealth = PlayerHealth + 70;
            if (PlayerHealth > PlayerHealthMax)
            {
                PlayerHealth = 100;
            }
            NumOfMedPacks--;
        }
    }
}

void AThePlayer::NetPlayParticleEffect_Implementation(UParticleSystem * ParticleSystem, FVector SpawnPoint)
{
    // Plays Particle Effect at the location that's passed in 
    UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, SpawnPoint);
}

void AThePlayer::OnDeath_Implementation()
{
    //SetActorLocation(StartPos);
    ClientDisableInput();
}

void AThePlayer::ServerRevival_Implementation()
{
    // Fire a ray trace line
    FHitResult OutHit;
    FVector Start = RifleSpawnBox->GetComponentLocation();
    FVector ForwardVector = FPSCamera->GetForwardVector();
    FVector End = ((ForwardVector * 50.0f) + Start);
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredComponent(this->GetCapsuleComponent());

    if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
    {
        if (OutHit.GetActor() != nullptr || OutHit.GetComponent() != nullptr)
        {
            End = OutHit.Location;
        }
    }
    // It hits a player and that player's health is 0 revive that player
    AThePlayer* player = Cast<AThePlayer>(OutHit.GetActor());
    if (player)
    {
        if (player->getHealth() <= 0)
        {
            if (NumOfMedPacks > 0)
            {
                player->SetHealth(25);
                ServerUseMedPackForRevival();
                player->ClientEnableInput();
            }
        }
    }
}

// Validate Functions
bool AThePlayer::ClientEnableInput_Validate()
{
    return true;
}
bool AThePlayer:: ClientDisableInput_Validate()
{
    return true;
}
bool AThePlayer::ServerRevival_Validate()
{
    return true;
}
bool AThePlayer::ServerUseMedPackForRevival_Validate()
{
    return true;
}

bool AThePlayer::SetHealth_Validate(int health)
{
    return true;
}
bool AThePlayer::NetStopAudio_Validate(UAudioComponent* AudioComponent)
{
    return true;
}
bool AThePlayer::NetPlayAudio_Validate(UAudioComponent* AudioComponent)
{
    return true;
}
bool AThePlayer::ServerStartSprint_Validate()
{
    return true;
}
bool AThePlayer::ServerStopSprint_Validate()
{
    return true;
}
bool AThePlayer::ServerSetSprintSpeed_Validate(float val)
{
    return true;
}
bool AThePlayer::ServerSubtractStamina_Validate(bool jump)
{
    return true;
}
bool AThePlayer::ServerReplenishStamina_Validate()
{
    return true;
}
bool AThePlayer::NetSetPlayerModelOff_Validate()
{
    return true;
}
bool AThePlayer::ServerFire_Validate()
{
    return true;
}
bool AThePlayer::NetLaser_Validate()
{
    return true;
}
bool AThePlayer::ServerReloadCurrentWeapon_Validate()
{
    return true;
}
bool AThePlayer::ClientVisibleWeapon_Validate()
{
    return true;
}
bool AThePlayer::NetRecoil_Validate()
{
    return true;
}
bool AThePlayer::ServerSwitchWeapon_Validate()
{
    return true;
}
bool AThePlayer::ServerApplyMonsterDamage_Validate(AMonsters* monster, UPrimitiveComponent * HitComp)
{
    return true;
}
bool AThePlayer::ServerApplyPlayerDamage_Validate(AActor * OtherActor)
{
    return true;
}
bool AThePlayer::ServerUseMedPack_Validate()
{
    return true;
}
bool AThePlayer::NetPlayParticleEffect_Validate(UParticleSystem * ParticleSystem, FVector SpawnPoint)
{
    return true;
}
bool AThePlayer::OnDeath_Validate()
{
    return true;
}

// Replicated Varibles
void AThePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AThePlayer, PlayerHealth);
    DOREPLIFETIME(AThePlayer, NumOfMedPacks);
    DOREPLIFETIME(AThePlayer, BulletImpact);
    DOREPLIFETIME(AThePlayer, PistolMuzzleFlash);
    DOREPLIFETIME(AThePlayer, RifleMuzzleFlash);
    DOREPLIFETIME(AThePlayer, FPSRifleMesh);
    DOREPLIFETIME(AThePlayer, FPSPistolMesh);
    DOREPLIFETIME(AThePlayer, ActiveGun);
    DOREPLIFETIME(AThePlayer, CurrentStamina);
    DOREPLIFETIME(AThePlayer, MaxWalkSpeed);
    DOREPLIFETIME(AThePlayer, SprintWalkSpeed);
    DOREPLIFETIME(AThePlayer, RegularWalkSpeed);
    DOREPLIFETIME(AThePlayer, RifleGun);
    DOREPLIFETIME(AThePlayer, PistolGun);
    DOREPLIFETIME(AThePlayer, bIsSprinting);
    DOREPLIFETIME(AThePlayer, PistolAudio);
    DOREPLIFETIME(AThePlayer, RifleAudio);
    DOREPLIFETIME(AThePlayer, RifleReloadAudio);
    DOREPLIFETIME(AThePlayer, PistolReloadAudio);
    DOREPLIFETIME(AThePlayer, SprintAudio);
    DOREPLIFETIME(AThePlayer, WalkAudio);
    DOREPLIFETIME(AThePlayer, ActiveMovementAudio);
    DOREPLIFETIME(AThePlayer, DamageTimer);
    DOREPLIFETIME(AThePlayer, LaserSpawn);
    DOREPLIFETIME(AThePlayer, DamageCapsule);
    DOREPLIFETIME(AThePlayer, PistolSpawnBox);
    DOREPLIFETIME(AThePlayer, RifleSpawnBox);
    DOREPLIFETIME(AThePlayer, MovementSoundLocation);
    DOREPLIFETIME(AThePlayer, bHasBlackBox);
}