#include "KaeganHome.h"

#include "Blueprint/WidgetTree.h"
#include "Camera/CameraComponent.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CapsuleComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/PointLight.h"
#include "Engine/DirectionalLight.h"
#include "Engine/Engine.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UnrealClient.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogKaeganHome, Log, All);

namespace KaeganHome
{
    static UStaticMesh* Cube()
    {
        return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    }

    static UMaterialInterface* BasicMaterial()
    {
        return LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    }

    static void Tint(UStaticMeshComponent* Component, const FLinearColor& Color)
    {
        if (!Component) return;
        if (UMaterialInterface* Material = BasicMaterial())
        {
            if (UMaterialInstanceDynamic* Dynamic = Component->CreateDynamicMaterialInstance(0, Material))
            {
                Dynamic->SetVectorParameterValue(TEXT("Color"), Color);
                // BasicShapeMaterial exposes Color. Explicitly reassign the MID so runtime geometry
                // never falls back to the source material when the component is rebuilt.
                Component->SetMaterial(0, Dynamic);
            }
        }
    }
}

void UKaeganHomeScreen::NativeConstruct()
{
    Super::NativeConstruct();
    UCanvasPanel* Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("KaeganCanvas"));
    WidgetTree->RootWidget = Canvas;

    Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("KaeganBlack"));
    Background->SetBrushColor(FLinearColor::Black);
    if (UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(Background))
    {
        CanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
        CanvasSlot->SetOffsets(FMargin(0.f));
    }

    BodyText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("KaeganBody"));
    BodyText->SetJustification(ETextJustify::Center);
    BodyText->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.9f, 1.f, 1.f)));
    FSlateFontInfo BodyFont = BodyText->GetFont();
    BodyFont.Size = 24;
    BodyText->SetFont(BodyFont);
    if (UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(BodyText))
    {
        CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
        CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
        CanvasSlot->SetPosition(FVector2D(0.f, -85.f));
        CanvasSlot->SetSize(FVector2D(820.f, 220.f));
    }

    ButtonBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("KaeganButtons"));
    if (UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(ButtonBox))
    {
        CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
        CanvasSlot->SetAlignment(FVector2D(0.5f, 0.f));
        CanvasSlot->SetPosition(FVector2D(0.f, 70.f));
        CanvasSlot->SetSize(FVector2D(280.f, 180.f));
    }
}

void UKaeganHomeScreen::Configure(EKaeganScreenMode InMode, const FText& InBody, AKaeganHomePlayerController* InOwner)
{
    Mode = InMode;
    OwnerController = InOwner;
    if (Background)
    {
        const float Alpha = Mode == EKaeganScreenMode::Notice ? 0.f : (Mode == EKaeganScreenMode::Pause ? 0.72f : 1.f);
        Background->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, Alpha));
    }
    SetBody(InBody);
    if (!ButtonBox) return;
    ButtonBox->ClearChildren();
    if (Mode == EKaeganScreenMode::Pause)
    {
        AddButton(FText::FromString(TEXT("RESUME")), 1);
        AddButton(FText::FromString(TEXT("RESTART LEVEL")), 2);
        AddButton(FText::FromString(TEXT("RETURN TO MAIN MENU")), 3);
    }
    else if (Mode == EKaeganScreenMode::Complete)
    {
        AddButton(FText::FromString(TEXT("CONTINUE")), 4);
        AddButton(FText::FromString(TEXT("RESTART LEVEL")), 2);
        AddButton(FText::FromString(TEXT("MAIN MENU")), 3);
    }
}

void UKaeganHomeScreen::SetBody(const FText& InBody)
{
    if (BodyText) BodyText->SetText(InBody);
}

void UKaeganHomeScreen::AddButton(const FText& Label, int32 Action)
{
    UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
    UTextBlock* LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    LabelText->SetText(Label);
    LabelText->SetJustification(ETextJustify::Center);
    LabelText->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.85f, 0.9f, 1.f)));
    Button->AddChild(LabelText);
    if (Action == 1) Button->OnClicked.AddDynamic(this, &UKaeganHomeScreen::ResumeClicked);
    if (Action == 2) Button->OnClicked.AddDynamic(this, &UKaeganHomeScreen::RestartClicked);
    if (Action == 3) Button->OnClicked.AddDynamic(this, &UKaeganHomeScreen::MenuClicked);
    if (Action == 4) Button->OnClicked.AddDynamic(this, &UKaeganHomeScreen::ContinueClicked);
    if (UVerticalBoxSlot* BoxSlot = ButtonBox->AddChildToVerticalBox(Button))
    {
        BoxSlot->SetPadding(FMargin(0.f, 5.f));
        BoxSlot->SetHorizontalAlignment(HAlign_Fill);
    }
}

void UKaeganHomeScreen::RestartClicked() { if (OwnerController) OwnerController->RestartLevel(); }
void UKaeganHomeScreen::ResumeClicked() { if (OwnerController) OwnerController->ResumeGame(); }
void UKaeganHomeScreen::MenuClicked() { if (OwnerController) OwnerController->ReturnToMenu(); }
void UKaeganHomeScreen::ContinueClicked() { if (OwnerController) OwnerController->ContinueUnavailable(); }

AKaeganInteractable::AKaeganInteractable()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    Mesh->SetStaticMesh(KaeganHome::Cube());
    Mesh->SetCollisionProfileName(TEXT("BlockAll"));
    PromptText = FText::FromString(TEXT("[E] Examine"));
}

FText AKaeganInteractable::GetPrompt() const { return bAvailable ? PromptText : FText::GetEmpty(); }

void AKaeganInteractable::Interact(AKaeganFirstPersonCharacter* Character)
{
    if (!bAvailable || !Character) return;
    bAvailable = false;
    if (AKaeganHomePlayerController* Controller = Cast<AKaeganHomePlayerController>(Character->GetController()))
    {
        Controller->ShowNotice(FText::FromString(TEXT("The house does not answer.")));
    }
}

AKaeganDoorActor::AKaeganDoorActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
    SetRootComponent(Hinge);
    Mesh->SetupAttachment(Hinge);
    Mesh->SetRelativeLocation(FVector(0.f, 80.f, 130.f));
    Mesh->SetRelativeScale3D(FVector(0.09f, 1.6f, 2.6f));
    KaeganHome::Tint(Mesh, FLinearColor(0.10f, 0.055f, 0.035f));
    PromptText = FText::FromString(TEXT("[E] Open door"));
}

FText AKaeganDoorActor::GetPrompt() const
{
    if (bLocked) return FText::FromString(TEXT("The door will not move."));
    return bOpen ? FText::FromString(TEXT("[E] Close door")) : FText::FromString(TEXT("[E] Open door"));
}

void AKaeganDoorActor::Interact(AKaeganFirstPersonCharacter* Character)
{
    if (bLocked || bMoving) return;
    SetOpen(!bOpen);
}

void AKaeganDoorActor::SetLocked(bool bInLocked) { bLocked = bInLocked; }

void AKaeganDoorActor::SetOpen(bool bInOpen)
{
    if (bLocked || bOpen == bInOpen) return;
    bOpen = bInOpen;
    bMoving = true;
    TargetAngle = bOpen ? OpenDirection : 0.f;
}

void AKaeganDoorActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bMoving) return;
    CurrentAngle = FMath::FInterpTo(CurrentAngle, TargetAngle, DeltaSeconds, 4.5f);
    Mesh->SetRelativeRotation(FRotator(0.f, CurrentAngle, 0.f));
    if (FMath::IsNearlyEqual(CurrentAngle, TargetAngle, 0.25f))
    {
        CurrentAngle = TargetAngle;
        Mesh->SetRelativeRotation(FRotator(0.f, CurrentAngle, 0.f));
        bMoving = false;
    }
}

FText AKaeganMemoryObject::GetPrompt() const
{
    return bAvailable ? FText::FromString(TEXT("[E] Examine the drawing")) : FText::GetEmpty();
}

void AKaeganMemoryObject::Interact(AKaeganFirstPersonCharacter* Character)
{
    if (!bAvailable || !Character) return;
    bAvailable = false;
    if (AKaeganHomePlayerController* Controller = Cast<AKaeganHomePlayerController>(Character->GetController()))
    {
        Character->SetMovementLocked(true);
        Controller->ShowNotice(FText::FromString(TEXT("I remember this.")), 2.2f);
        FTimerHandle UnlockTimer;
        Character->GetWorldTimerManager().SetTimer(UnlockTimer, [Character]() { if (IsValid(Character)) Character->SetMovementLocked(false); }, 2.2f, false);
    }
    for (TActorIterator<AKaeganHomeDirector> It(GetWorld()); It; ++It)
    {
        It->AdvanceFromMemory();
        break;
    }
}

AKaeganFatherMimic::AKaeganFatherMimic()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TEMP_FatherMimic"));
    SetRootComponent(SceneRoot);
    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TEMP_FatherMimic_Body"));
    Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TEMP_FatherMimic_Head"));
    LeftCrutch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TEMP_LeftCrutch"));
    RightCrutch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TEMP_RightCrutch"));
    for (UStaticMeshComponent* Piece : { Body, Head, LeftCrutch, RightCrutch })
    {
        Piece->SetupAttachment(SceneRoot);
        Piece->SetStaticMesh(KaeganHome::Cube());
        Piece->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        KaeganHome::Tint(Piece, FLinearColor(0.028f, 0.040f, 0.065f));
    }
    Body->SetRelativeLocation(FVector(0.f, 0.f, 112.f));
    Body->SetRelativeScale3D(FVector(0.45f, 0.34f, 2.15f));
    Head->SetRelativeLocation(FVector(0.f, 0.f, 236.f));
    Head->SetRelativeScale3D(FVector(0.52f, 0.48f, 0.52f));
    LeftCrutch->SetRelativeLocation(FVector(0.f, -60.f, 108.f));
    LeftCrutch->SetRelativeRotation(FRotator(8.f, 0.f, -12.f));
    LeftCrutch->SetRelativeScale3D(FVector(0.07f, 0.07f, 2.2f));
    RightCrutch->SetRelativeLocation(FVector(-18.f, 68.f, 96.f));
    RightCrutch->SetRelativeRotation(FRotator(-9.f, 0.f, 18.f));
    RightCrutch->SetRelativeScale3D(FVector(0.07f, 0.07f, 2.0f));
    SetRevealed(false);
}

void AKaeganFatherMimic::SetRevealed(bool bReveal)
{
    SetActorHiddenInGame(!bReveal);
    SetActorEnableCollision(false);
}

void AKaeganFatherMimic::Relocate(const FVector& Position) { SetActorLocation(Position); }

AKaeganFirstPersonCharacter::AKaeganFirstPersonCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);
    GetCharacterMovement()->MaxWalkSpeed = 185.f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 120.f;
    GetCharacterMovement()->JumpZVelocity = 0.f;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    Camera->SetupAttachment(GetCapsuleComponent());
    Camera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
    Camera->bUsePawnControlRotation = true;
}

void AKaeganFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AKaeganFirstPersonCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AKaeganFirstPersonCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AKaeganFirstPersonCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AKaeganFirstPersonCharacter::LookUp);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AKaeganFirstPersonCharacter::TryInteract);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AKaeganFirstPersonCharacter::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AKaeganFirstPersonCharacter::StopSprint);
}

void AKaeganFirstPersonCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    TraceAccumulator += DeltaSeconds;
    if (TraceAccumulator >= 0.08f)
    {
        TraceAccumulator = 0.f;
        UpdateInteraction();
    }
}

void AKaeganFirstPersonCharacter::MoveForward(float Value) { if (!bMovementLocked && FMath::Abs(Value) > KINDA_SMALL_NUMBER) AddMovementInput(GetActorForwardVector(), Value); }
void AKaeganFirstPersonCharacter::MoveRight(float Value) { if (!bMovementLocked && FMath::Abs(Value) > KINDA_SMALL_NUMBER) AddMovementInput(GetActorRightVector(), Value); }
void AKaeganFirstPersonCharacter::Turn(float Value) { if (!bMovementLocked) AddControllerYawInput(Value); }
void AKaeganFirstPersonCharacter::LookUp(float Value) { if (!bMovementLocked) AddControllerPitchInput(Value); }
void AKaeganFirstPersonCharacter::StartSprint() { if (!bMovementLocked) { bSprinting = true; GetCharacterMovement()->MaxWalkSpeed = 260.f; } }
void AKaeganFirstPersonCharacter::StopSprint() { bSprinting = false; GetCharacterMovement()->MaxWalkSpeed = 185.f; }

void AKaeganFirstPersonCharacter::SetMovementLocked(bool bLocked)
{
    bMovementLocked = bLocked;
    if (bLocked) GetCharacterMovement()->DisableMovement();
    else GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AKaeganFirstPersonCharacter::UpdateInteraction()
{
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(KaeganInteraction), false, this);
    const FVector Start = Camera->GetComponentLocation();
    const FVector End = Start + Camera->GetForwardVector() * 330.f;
    AKaeganInteractable* NewFocus = nullptr;
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        NewFocus = Cast<AKaeganInteractable>(Hit.GetActor());
        if (NewFocus && !NewFocus->IsAvailable()) NewFocus = nullptr;
    }
    FocusedInteractable = NewFocus;
    if (AKaeganHomePlayerController* HomeController = Cast<AKaeganHomePlayerController>(GetController()))
    {
        HomeController->SetInteractionPrompt(NewFocus ? NewFocus->GetPrompt() : FText::GetEmpty());
    }
}

void AKaeganFirstPersonCharacter::TryInteract()
{
    if (bMovementLocked) return;
    if (AKaeganInteractable* Target = FocusedInteractable.Get()) Target->Interact(this);
}

void AKaeganHomePlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
}

void AKaeganHomePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    FInputActionBinding& PauseBinding = InputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &AKaeganHomePlayerController::TogglePauseMenu);
    PauseBinding.bExecuteWhenPaused = true;
}

void AKaeganHomePlayerController::ClearScreen()
{
    GetWorldTimerManager().ClearTimer(NoticeTimer);
    if (ActiveScreen)
    {
        ActiveScreen->RemoveFromParent();
        ActiveScreen = nullptr;
    }
}

void AKaeganHomePlayerController::ShowBoot()
{
    ClearScreen();
    ActiveScreen = CreateWidget<UKaeganHomeScreen>(this, UKaeganHomeScreen::StaticClass());
    ActiveScreen->AddToViewport(50);
    ActiveScreen->Configure(EKaeganScreenMode::Boot, FText::FromString(TEXT("KAEGAN.EXE\n\nBUILD STATUS: INCOMPLETE\nMEMORY SOURCE DETECTED\nLOADING: HOME")), this);
}

void AKaeganHomePlayerController::ShowPause()
{
    ClearScreen();
    ActiveScreen = CreateWidget<UKaeganHomeScreen>(this, UKaeganHomeScreen::StaticClass());
    ActiveScreen->AddToViewport(60);
    ActiveScreen->Configure(EKaeganScreenMode::Pause, FText::FromString(TEXT("PAUSED\n\nThe house waits.")), this);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
    SetPause(true);
}

void AKaeganHomePlayerController::ShowCompletion()
{
    ClearScreen();
    ActiveScreen = CreateWidget<UKaeganHomeScreen>(this, UKaeganHomeScreen::StaticClass());
    ActiveScreen->AddToViewport(80);
    ActiveScreen->Configure(EKaeganScreenMode::Complete, FText::FromString(TEXT("MEMORY 01 RECOVERED\nSOURCE INCONSISTENT\nFATHER INSTANCE DETECTED")), this);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void AKaeganHomePlayerController::ShowNotice(const FText& Text, float Duration)
{
    ClearScreen();
    ActiveScreen = CreateWidget<UKaeganHomeScreen>(this, UKaeganHomeScreen::StaticClass());
    ActiveScreen->AddToViewport(45);
    ActiveScreen->Configure(EKaeganScreenMode::Notice, Text, this);
    GetWorldTimerManager().SetTimer(NoticeTimer, this, &AKaeganHomePlayerController::ClearScreen, Duration, false);
}

void AKaeganHomePlayerController::SetInteractionPrompt(const FText& Text)
{
    const FString NewText = Text.ToString();
    if (NewText == LastPrompt) return;
    LastPrompt = NewText;
    if (!NewText.IsEmpty() && GEngine) GEngine->AddOnScreenDebugMessage(1337, 0.13f, FColor(185, 205, 210), NewText);
}

void AKaeganHomePlayerController::TogglePauseMenu() { IsPaused() ? ResumeGame() : ShowPause(); }
void AKaeganHomePlayerController::RestartLevel() { SetPause(false); UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true))); }
void AKaeganHomePlayerController::ReturnToMenu() { RestartLevel(); }
void AKaeganHomePlayerController::ResumeGame() { ClearScreen(); bShowMouseCursor = false; SetInputMode(FInputModeGameOnly()); SetPause(false); }
void AKaeganHomePlayerController::ContinueUnavailable()
{
    ShowNotice(FText::FromString(TEXT("NEXT MEMORY UNAVAILABLE")), 1.8f);
    FTimerHandle ReturnTimer;
    GetWorldTimerManager().SetTimer(ReturnTimer, this, &AKaeganHomePlayerController::ReturnToMenu, 2.1f, false);
}

AKaeganHomeDirector::AKaeganHomeDirector()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AKaeganHomeDirector::BeginPlay()
{
    Super::BeginPlay();
    BuildHome();
    if (AKaeganFirstPersonCharacter* Kaegan = GetKaegan()) Kaegan->SetMovementLocked(true);
    if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowBoot();
    GetWorldTimerManager().SetTimer(BootTimer, this, &AKaeganHomeDirector::FinishBoot, 5.8f, false);
    GetWorldTimerManager().SetTimer(ProgressTimer, this, &AKaeganHomeDirector::UpdateProgression, 0.25f, true);
    UE_LOG(LogKaeganHome, Log, TEXT("LEVEL 01 HOME: runtime house constructed."));
    if (FParse::Param(FCommandLine::Get(), TEXT("KaeganValidation")))
    {
        RunAutomatedValidation();
    }
}

void AKaeganHomeDirector::SetState(EKaeganLevelState NewState)
{
    if (State == NewState) return;
    State = NewState;
    UE_LOG(LogKaeganHome, Log, TEXT("LEVEL 01 HOME state -> %d"), static_cast<int32>(State));
}

void AKaeganHomeDirector::FinishBoot()
{
    SetState(EKaeganLevelState::ExploreHouse);
    if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ClearScreen();
    if (AKaeganFirstPersonCharacter* Kaegan = GetKaegan()) Kaegan->SetMovementLocked(false);
    if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowNotice(FText::FromString(TEXT("Find the room that remembers you.")), 2.4f);
    if (FParse::Param(FCommandLine::Get(), TEXT("KaeganCapture")))
    {
        GetWorldTimerManager().SetTimer(StartingScreenshotTimer, FTimerDelegate::CreateWeakLambda(this, [this]()
        {
            RequestValidationScreenshot(TEXT("KaeganHome_StartingRoom"));
        }), 2.6f, false);
    }
}

AStaticMeshActor* AKaeganHomeDirector::MakeBox(const FVector& Location, const FVector& Dimensions, const FLinearColor& Color, const FString& Label, bool bCollision)
{
    AStaticMeshActor* Box = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
    if (!Box) return nullptr;
    UStaticMeshComponent* Comp = Box->GetStaticMeshComponent();
    Comp->SetMobility(EComponentMobility::Movable);
    Comp->SetStaticMesh(KaeganHome::Cube());
    Comp->SetCollisionEnabled(bCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    Comp->SetCollisionResponseToAllChannels(bCollision ? ECR_Block : ECR_Ignore);
    Box->SetActorScale3D(Dimensions / 100.f);
    Box->Tags.Add(FName(*Label));
    KaeganHome::Tint(Comp, Color);
#if !UE_BUILD_SHIPPING
    if (Label == TEXT("LivingRoom_Floor"))
    {
        if (UMaterialInstanceDynamic* Dynamic = Cast<UMaterialInstanceDynamic>(Comp->GetMaterial(0)))
        {
            UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VISIBILITY_VALIDATION: material_tint=%s expected=%s"), *Dynamic->K2_GetVectorParameterValue(TEXT("Color")).ToString(), *Color.ToString());
        }
        else
        {
            UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VISIBILITY_VALIDATION: LivingRoom floor did not retain a dynamic tint material."));
        }
    }
#endif
    return Box;
}

APointLight* AKaeganHomeDirector::MakeLight(const FVector& Location, const FLinearColor& Color, float Intensity, float Radius, const FString& Label, bool bCastShadows)
{
    APointLight* Light = GetWorld()->SpawnActor<APointLight>(Location, FRotator::ZeroRotator);
    if (!Light) return nullptr;
    UPointLightComponent* Component = Cast<UPointLightComponent>(Light->GetLightComponent());
    if (!Component) return nullptr;
    Component->SetMobility(EComponentMobility::Movable);
    Component->SetLightColor(Color);
    Component->SetIntensity(Intensity);
    Component->SetAttenuationRadius(Radius);
    Component->SetCastShadows(bCastShadows);
    Light->Tags.Add(FName(*Label));
    HouseLights.Add(Light);
    return Light;
}

void AKaeganHomeDirector::ConfigureVisibilityPass()
{
    // The house is assembled at runtime, so a runtime volume guarantees that editor viewport
    // settings and inherited map exposure cannot turn a playable room into a black frame.
    VisibilityPostProcess = GetWorld()->SpawnActor<APostProcessVolume>(FVector::ZeroVector, FRotator::ZeroRotator);
    if (VisibilityPostProcess)
    {
        VisibilityPostProcess->bUnbound = true;
        VisibilityPostProcess->Priority = 100.f;
        VisibilityPostProcess->BlendWeight = 1.f;
        VisibilityPostProcess->Settings.bOverride_AutoExposureMethod = true;
        VisibilityPostProcess->Settings.AutoExposureMethod = AEM_Manual;
        VisibilityPostProcess->Settings.bOverride_AutoExposureBias = true;
        VisibilityPostProcess->Settings.AutoExposureBias = 1.2f;
        VisibilityPostProcess->Settings.bOverride_AutoExposureApplyPhysicalCameraExposure = true;
        VisibilityPostProcess->Settings.AutoExposureApplyPhysicalCameraExposure = false;
        VisibilityPostProcess->Settings.bOverride_VignetteIntensity = true;
        VisibilityPostProcess->Settings.VignetteIntensity = 0.18f;
        VisibilityPostProcess->Settings.bOverride_AmbientOcclusionIntensity = true;
        VisibilityPostProcess->Settings.AmbientOcclusionIntensity = 0.55f;
    }

    // A restrained blue skylight and shadowless moon fill keep wall/floor planes readable.
    // The practical point lights below still provide the primary contrast and cast shadows.
    AmbientSkyLight = GetWorld()->SpawnActor<ASkyLight>(FVector(500.f, 0.f, 500.f), FRotator::ZeroRotator);
    if (USkyLightComponent* Sky = AmbientSkyLight ? AmbientSkyLight->GetLightComponent() : nullptr)
    {
        Sky->SetMobility(EComponentMobility::Movable);
        Sky->SetLightColor(FLinearColor(0.16f, 0.22f, 0.34f));
        Sky->SetIntensity(0.55f);
        Sky->bRealTimeCapture = false;
        Sky->bLowerHemisphereIsBlack = true;
        Sky->SetLowerHemisphereColor(FLinearColor(0.035f, 0.055f, 0.09f));
        Sky->RecaptureSky();
    }

    Moonlight = GetWorld()->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-38.f, -118.f, 0.f));
    if (UDirectionalLightComponent* Directional = Moonlight ? Moonlight->GetComponent() : nullptr)
    {
        Directional->SetMobility(EComponentMobility::Movable);
        Directional->SetLightColor(FLinearColor(0.20f, 0.30f, 0.52f));
        Directional->SetIntensity(0.45f);
        Directional->SetCastShadows(false);
    }
}

AKaeganDoorActor* AKaeganHomeDirector::MakeDoor(const FVector& Location, float Yaw, const FString& Label, bool bLocked)
{
    AKaeganDoorActor* Door = GetWorld()->SpawnActor<AKaeganDoorActor>(Location, FRotator(0.f, Yaw, 0.f));
    if (Door)
    {
        Door->Tags.Add(FName(*Label));
        Door->SetLocked(bLocked);
    }
    return Door;
}

void AKaeganHomeDirector::BuildHome()
{
    if (bBuilt) return;
    bBuilt = true;
    // These are dark albedos, but no longer so near-black that a correctly lit room loses its planes.
    const FLinearColor Floor(0.30f, 0.23f, 0.17f);
    const FLinearColor Wall(0.36f, 0.40f, 0.42f);
    const FLinearColor Ceiling(0.24f, 0.28f, 0.34f);
    const FLinearColor Wood(0.24f, 0.135f, 0.075f);
    auto FloorRoom = [this, &Floor, &Ceiling](FVector Center, FVector Size, const FString& Name)
    {
        MakeBox(Center + FVector(0, 0, -12), FVector(Size.X, Size.Y, 24), Floor, Name + TEXT("_Floor"));
        MakeBox(Center + FVector(0, 0, 312), FVector(Size.X, Size.Y, 20), Ceiling, Name + TEXT("_Ceiling"));
    };
    auto WallX = [this, &Wall](float X, float Y, float Length, const FString& Name) { MakeBox(FVector(X, Y, 150), FVector(18, Length, 300), Wall, Name); };
    auto WallY = [this, &Wall](float X, float Y, float Length, const FString& Name) { MakeBox(FVector(X, Y, 150), FVector(Length, 18, 300), Wall, Name); };

    // Entry/living room, kitchen, the long family hallway, bathroom, and two bedrooms form one coherent memory-house route.
    FloorRoom(FVector(500, 0, 0), FVector(1700, 1400, 0), TEXT("LivingRoom"));
    FloorRoom(FVector(500, -1200, 0), FVector(1600, 1000, 0), TEXT("KitchenDining"));
    FloorRoom(FVector(2050, 0, 0), FVector(1500, 500, 0), TEXT("MainHall"));
    FloorRoom(FVector(2700, 900, 0), FVector(1100, 1000, 0), TEXT("KaeganRoom"));
    FloorRoom(FVector(2050, -850, 0), FVector(700, 900, 0), TEXT("Bathroom"));
    FloorRoom(FVector(3000, -850, 0), FVector(1000, 900, 0), TEXT("ParentRoom"));
    FloorRoom(FVector(3650, 0, 0), FVector(1600, 500, 0), TEXT("FinalHall"));

    WallX(-350, 0, 1400, TEXT("Living_West")); WallY(500, 700, 1700, TEXT("Living_North"));
    WallY(-75, -700, 550, TEXT("KitchenDoorWall_A")); WallY(1075, -700, 550, TEXT("KitchenDoorWall_B"));
    WallX(-300, -1200, 1000, TEXT("Kitchen_West")); WallX(1300, -1200, 1000, TEXT("Kitchen_East")); WallY(500, -1700, 1600, TEXT("Kitchen_South"));
    WallY(2050, 250, 1500, TEXT("Hall_North")); WallY(2050, -250, 1500, TEXT("Hall_South"));
    WallY(2295, 400, 290, TEXT("KaeganDoorWall_A")); WallY(2925, 400, 650, TEXT("KaeganDoorWall_B"));
    WallY(2700, 1400, 1100, TEXT("Kaegan_North")); WallX(2150, 900, 1000, TEXT("Kaegan_West")); WallX(3250, 900, 1000, TEXT("Kaegan_East"));
    WallX(1700, -850, 900, TEXT("Bath_West")); WallX(2400, -850, 900, TEXT("Bath_East")); WallY(2050, -1300, 700, TEXT("Bath_South"));
    WallY(3000, -400, 1000, TEXT("Parent_North")); WallY(3000, -1300, 1000, TEXT("Parent_South")); WallX(3500, -850, 900, TEXT("Parent_East"));
    WallY(3650, 250, 1600, TEXT("FinalHall_North")); WallY(3650, -250, 1600, TEXT("FinalHall_South")); WallX(4450, 0, 500, TEXT("FinalHall_End"));
    WallX(1350, -125, 250, TEXT("LivingDoorWall_A")); WallX(1350, 205, 90, TEXT("LivingDoorWall_B"));
    WallX(2800, -125, 250, TEXT("FinalGateWall_A")); WallX(2800, 205, 90, TEXT("FinalGateWall_B"));

    MakeDoor(FVector(1350, 0, 0), 0.f, TEXT("Door_LivingToHall"));
    MakeDoor(FVector(2600, 400, 0), 90.f, TEXT("Door_KaeganRoom"));
    MakeDoor(FVector(2400, -850, 0), 0.f, TEXT("Door_Bathroom"));
    FinalGate = MakeDoor(FVector(2800, 0, 0), 0.f, TEXT("Door_FinalHall"), true);

    // Furniture silhouettes and domestic landmarks. TEMP assets are intentionally simple and replaceable.
    MakeBox(FVector(240, 250, 55), FVector(350, 150, 90), Wood, TEXT("TEMP_LivingSofa"));
    MakeBox(FVector(740, 230, 70), FVector(170, 80, 140), FLinearColor(0.055f, 0.075f, 0.10f), TEXT("TEMP_Television"));
    MakeBox(FVector(620, -1200, 90), FVector(330, 170, 100), Wood, TEXT("TEMP_KitchenTable"));
    MakeBox(FVector(200, -1580, 110), FVector(850, 80, 170), Wood, TEXT("TEMP_KitchenCounters"));
    MakeBox(FVector(2850, 980, 55), FVector(360, 220, 90), FLinearColor(0.17f, 0.21f, 0.27f), TEXT("TEMP_KaeganBed"));
    MakeBox(FVector(2920, 650, 130), FVector(140, 45, 230), Wood, TEXT("TEMP_KaeganDesk"));
    MakeBox(FVector(2070, -900, 95), FVector(120, 80, 170), FLinearColor(0.70f, 0.72f, 0.7f), TEXT("TEMP_BathroomMirror"), false);
    CorruptionProps.Add(MakeBox(FVector(3575, 120, 110), FVector(100, 280, 220), FLinearColor(0.045f, 0.075f, 0.10f), TEXT("CorruptedFurniture")));
    CorruptionProps.Last()->SetActorHiddenInGame(true);

    MemoryObject = GetWorld()->SpawnActor<AKaeganMemoryObject>(FVector(2920, 590, 180), FRotator(0.f, 90.f, 0.f));
    if (MemoryObject)
    {
        MemoryObject->Tags.Add(TEXT("TEMP_KaeganDrawing"));
        MemoryObject->GetMesh()->SetRelativeScale3D(FVector(0.08f, 1.2f, 0.85f));
        KaeganHome::Tint(MemoryObject->GetMesh(), FLinearColor(0.75f, 0.63f, 0.35f));
    }
    AKaeganInteractable* Television = GetWorld()->SpawnActor<AKaeganInteractable>(FVector(705, 180, 145), FRotator::ZeroRotator);
    if (Television)
    {
        Television->ConfigureInteraction(FText::FromString(TEXT("[E] Touch the television")));
        Television->GetMesh()->SetRelativeScale3D(FVector(1.25f, 0.18f, 0.8f));
        KaeganHome::Tint(Television->GetMesh(), FLinearColor(0.06f, 0.11f, 0.13f));
    }

    // Shadowed practicals define the horror contrast; broad shadowless fills only reveal navigation.
    MakeLight(FVector(430, -80, 255), FLinearColor(0.54f, 0.66f, 0.88f), 4200.f, 1550.f, TEXT("Practical_LivingMoonlight"));
    MakeLight(FVector(500, 100, 175), FLinearColor(0.16f, 0.23f, 0.36f), 2600.f, 1850.f, TEXT("Ambient_LivingFill"), false);
    MakeLight(FVector(500, -1180, 255), FLinearColor(0.92f, 0.60f, 0.30f), 3200.f, 1250.f, TEXT("Practical_KitchenWarmth"));
    MakeLight(FVector(450, -1150, 150), FLinearColor(0.18f, 0.24f, 0.34f), 2200.f, 1450.f, TEXT("Ambient_KitchenFill"), false);
    MakeLight(FVector(1950, 0, 225), FLinearColor(0.43f, 0.55f, 0.78f), 2800.f, 1150.f, TEXT("Practical_MainHall"));
    MakeLight(FVector(2200, 0, 155), FLinearColor(0.15f, 0.21f, 0.31f), 2300.f, 1250.f, TEXT("Ambient_MainHallFill"), false);
    MakeLight(FVector(2730, 900, 250), FLinearColor(0.40f, 0.52f, 0.80f), 2600.f, 1150.f, TEXT("Practical_KaeganRoom"));
    MakeLight(FVector(2780, 850, 150), FLinearColor(0.14f, 0.20f, 0.32f), 2000.f, 1200.f, TEXT("Ambient_KaeganRoomFill"), false);
    MakeLight(FVector(2050, -850, 235), FLinearColor(0.74f, 0.57f, 0.38f), 1900.f, 900.f, TEXT("Practical_Bathroom"));
    MakeLight(FVector(3000, -850, 220), FLinearColor(0.32f, 0.42f, 0.66f), 1800.f, 1150.f, TEXT("Practical_ParentRoom"));
    MakeLight(FVector(3600, 0, 220), FLinearColor(0.30f, 0.42f, 0.68f), 2200.f, 1450.f, TEXT("Practical_FinalHall"));
    MakeLight(FVector(3850, 0, 145), FLinearColor(0.11f, 0.17f, 0.28f), 2100.f, 1650.f, TEXT("Ambient_FinalHallFill"), false);

    ConfigureVisibilityPass();

    Mimic = GetWorld()->SpawnActor<AKaeganFatherMimic>(FVector(4200, 0, 0), FRotator(0.f, 180.f, 0.f));
    if (Mimic) Mimic->SetRevealed(false);
}

AKaeganFirstPersonCharacter* AKaeganHomeDirector::GetKaegan() const
{
    return Cast<AKaeganFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AKaeganHomeDirector::AdvanceFromMemory()
{
    if (State != EKaeganLevelState::ExploreHouse) return;
    SetState(EKaeganLevelState::FootstepsActive);
    if (FinalGate) FinalGate->SetLocked(false);
    for (APointLight* Light : HouseLights)
    {
        if (UPointLightComponent* Component = Light ? Cast<UPointLightComponent>(Light->GetLightComponent()) : nullptr)
        {
            const bool bAmbientFill = Light->Tags.Num() > 0 && Light->Tags[0].ToString().StartsWith(TEXT("Ambient_"));
            Component->SetIntensity(Component->Intensity * (bAmbientFill ? 0.88f : 0.76f));
        }
    }
    SpawnFootstepHint();
    GetWorldTimerManager().SetTimer(FootstepTimer, this, &AKaeganHomeDirector::SpawnFootstepHint, 5.5f, true, 6.f);
    GetWorldTimerManager().SetTimerForNextTick(this, &AKaeganHomeDirector::RevealDoorwayFigure);
}

void AKaeganHomeDirector::RevealDoorwayFigure()
{
    if (!Mimic || State != EKaeganLevelState::FootstepsActive) return;
    Mimic->Relocate(FVector(1950, 0, 0));
    Mimic->SetRevealed(true);
    SetState(EKaeganLevelState::MimicRevealed);
    FTimerHandle HideTimer;
    GetWorldTimerManager().SetTimer(HideTimer, this, &AKaeganHomeDirector::HideDoorwayFigure, 3.5f, false);
}

void AKaeganHomeDirector::HideDoorwayFigure()
{
    if (!Mimic) return;
    Mimic->SetRevealed(false);
    Mimic->Relocate(FVector(4200, 0, 0));
    if (State == EKaeganLevelState::MimicRevealed) SetState(EKaeganLevelState::FootstepsActive);
}

void AKaeganHomeDirector::SpawnFootstepHint()
{
    if (State != EKaeganLevelState::FootstepsActive && State != EKaeganLevelState::HouseCorrupted && State != EKaeganLevelState::MimicRevealed) return;
    const FVector Locations[] = { FVector(2050, -160, 30), FVector(2350, 180, 30), FVector(3600, -150, 30) };
    const FVector Location = Locations[FMath::RandRange(0, UE_ARRAY_COUNT(Locations) - 1)];
    UE_LOG(LogKaeganHome, Verbose, TEXT("TEMP_Footstep cue at %s"), *Location.ToString());
    if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowNotice(FText::FromString(TEXT("...clink     ...clunk")), 1.2f);
}

void AKaeganHomeDirector::RunAutomatedValidation()
{
    const bool bPawnReady = GetKaegan() != nullptr;
    UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VALIDATION: boot state=%d pawn=%s"), static_cast<int32>(State), bPawnReady ? TEXT("ready") : TEXT("missing"));
    LogVisibilityValidation(TEXT("boot"));
    if (!bPawnReady)
    {
        UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VALIDATION: failed before boot because the custom pawn was not possessed."));
        return;
    }
    GetWorldTimerManager().SetTimer(ValidationTimer, this, &AKaeganHomeDirector::ValidateMemoryInteraction, 6.3f, false);
}

void AKaeganHomeDirector::ValidateMemoryInteraction()
{
    AKaeganFirstPersonCharacter* Kaegan = GetKaegan();
    if (State != EKaeganLevelState::ExploreHouse || !MemoryObject || !Kaegan)
    {
        UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VALIDATION: memory setup failed state=%d memory=%s pawn=%s"), static_cast<int32>(State), MemoryObject ? TEXT("ready") : TEXT("missing"), Kaegan ? TEXT("ready") : TEXT("missing"));
        return;
    }
    if (AKaeganHomePlayerController* Controller = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
    {
        Controller->ShowPause();
        const bool bPauseMenuWorked = Controller->IsPaused() && Controller->HasActiveScreen();
        Controller->ResumeGame();
        if (!bPauseMenuWorked || Controller->IsPaused() || Controller->HasActiveScreen())
        {
            UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VALIDATION: pause/resume UI lifecycle failed."));
            return;
        }
        UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VALIDATION: pause/resume UI lifecycle passed."));
    }
    MemoryObject->Interact(Kaegan);
    UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VALIDATION: memory interaction advanced state=%d"), static_cast<int32>(State));
    Kaegan->SetActorLocationAndRotation(FVector(1550.f, 0.f, 100.f), FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
    if (FParse::Param(FCommandLine::Get(), TEXT("KaeganCapture")))
    {
        GetWorldTimerManager().SetTimer(HallwayScreenshotTimer, FTimerDelegate::CreateWeakLambda(this, [this]()
        {
            RequestValidationScreenshot(TEXT("KaeganHome_MainHall_AfterMemory"));
        }), 2.5f, false);
    }
    GetWorldTimerManager().SetTimer(ValidationTimer, this, &AKaeganHomeDirector::ValidateHallwayCorruption, 2.8f, false);
}

void AKaeganHomeDirector::RequestValidationScreenshot(const FString& Name) const
{
#if !UE_BUILD_SHIPPING
    const FString Path = FPaths::ProjectSavedDir() / TEXT("Screenshots") / Name;
    FScreenshotRequest::RequestScreenshot(Path, false, true);
    UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VISIBILITY_VALIDATION: requested screenshot %s"), *Path);
#endif
}

void AKaeganHomeDirector::LogVisibilityValidation(const TCHAR* Phase) const
{
#if !UE_BUILD_SHIPPING
    const AKaeganFirstPersonCharacter* Kaegan = GetKaegan();
    const FVector SpawnLocation = Kaegan ? Kaegan->GetActorLocation() : FVector::ZeroVector;
    UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VISIBILITY_VALIDATION: phase=%s player_spawn=%s"), Phase, *SpawnLocation.ToString());

    int32 ActiveLights = 0;
    for (const APointLight* Light : HouseLights)
    {
        const UPointLightComponent* Component = Light ? Cast<UPointLightComponent>(Light->GetLightComponent()) : nullptr;
        if (!Component) continue;
        const bool bActive = Component->IsVisible() && Component->Intensity > 0.f;
        ActiveLights += bActive ? 1 : 0;
        const FString Label = Light->Tags.Num() > 0 ? Light->Tags[0].ToString() : Light->GetName();
        UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VISIBILITY_VALIDATION: light=%s intensity=%.0f radius=%.0f shadows=%s active=%s"), *Label, Component->Intensity, Component->AttenuationRadius, Component->CastShadows ? TEXT("true") : TEXT("false"), bActive ? TEXT("true") : TEXT("false"));
    }
    UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VISIBILITY_VALIDATION: active_point_lights=%d ambient_sky=%s moon_fill=%s"), ActiveLights, AmbientSkyLight ? TEXT("present") : TEXT("missing"), Moonlight ? TEXT("present") : TEXT("missing"));

    if (VisibilityPostProcess)
    {
        UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VISIBILITY_VALIDATION: exposure=manual bias=%.2f physical_camera=%s volume_unbound=%s"), VisibilityPostProcess->Settings.AutoExposureBias, VisibilityPostProcess->Settings.AutoExposureApplyPhysicalCameraExposure ? TEXT("true") : TEXT("false"), VisibilityPostProcess->bUnbound ? TEXT("true") : TEXT("false"));
    }
    else
    {
        UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VISIBILITY_VALIDATION: exposure volume missing."));
    }

    if (Kaegan)
    {
        FCollisionQueryParams Params(SCENE_QUERY_STAT(KaeganSpawnClearance), false, Kaegan);
        const bool bSpawnBlocked = GetWorld()->OverlapBlockingTestByChannel(SpawnLocation, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeCapsule(34.f, 88.f), Params);
        const AActor* NearestBlocker = nullptr;
        float NearestDistance = TNumericLimits<float>::Max();
        for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
        {
            UStaticMeshComponent* Mesh = It->GetStaticMeshComponent();
            if (!Mesh || Mesh->GetCollisionEnabled() == ECollisionEnabled::NoCollision) continue;
            const float Distance = FMath::Sqrt(It->GetComponentsBoundingBox(true).ComputeSquaredDistanceToPoint(SpawnLocation));
            if (Distance < NearestDistance)
            {
                NearestDistance = Distance;
                NearestBlocker = *It;
            }
        }
        const FString NearestName = NearestBlocker && NearestBlocker->Tags.Num() > 0 ? NearestBlocker->Tags[0].ToString() : (NearestBlocker ? NearestBlocker->GetName() : TEXT("none"));
        UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VISIBILITY_VALIDATION: spawn_blocked=%s nearest_blocking_geometry=%s distance_cm=%.1f"), bSpawnBlocked ? TEXT("true") : TEXT("false"), *NearestName, NearestDistance);
    }
#endif
}

void AKaeganHomeDirector::ValidateHallwayCorruption()
{
    if (AKaeganFirstPersonCharacter* Kaegan = GetKaegan())
    {
        Kaegan->SetActorLocation(FVector(3100.f, 0.f, 100.f), false, nullptr, ETeleportType::TeleportPhysics);
        UpdateProgression();
    }
    if (State != EKaeganLevelState::HouseCorrupted)
    {
        UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VALIDATION: final hallway did not corrupt; state=%d"), static_cast<int32>(State));
        return;
    }
    UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VALIDATION: hallway corruption and final mimic staging passed."));
    GetWorldTimerManager().SetTimer(ValidationTimer, this, &AKaeganHomeDirector::ValidateEnding, 0.4f, false);
}

void AKaeganHomeDirector::ValidateEnding()
{
    if (AKaeganFirstPersonCharacter* Kaegan = GetKaegan())
    {
        Kaegan->SetActorLocation(FVector(4050.f, 0.f, 100.f), false, nullptr, ETeleportType::TeleportPhysics);
        UpdateProgression();
    }
    if (State != EKaeganLevelState::FinalSequence)
    {
        UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VALIDATION: ending did not start; state=%d"), static_cast<int32>(State));
        return;
    }
    UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VALIDATION: final sequence entered and movement lock applied."));
    GetWorldTimerManager().SetTimer(ValidationTimer, this, &AKaeganHomeDirector::ValidateCompletion, 5.0f, false);
}

void AKaeganHomeDirector::ValidateCompletion()
{
    const bool bComplete = State == EKaeganLevelState::LevelComplete;
    const AKaeganHomePlayerController* Controller = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    const bool bScreenReady = Controller && Controller->HasActiveScreen() && Controller->bShowMouseCursor;
    if (bComplete && bScreenReady)
    {
        UE_LOG(LogKaeganHome, Log, TEXT("KAEGAN_VALIDATION: completion state=%d controller=%s"), static_cast<int32>(State), TEXT("ready"));
    }
    else
    {
        UE_LOG(LogKaeganHome, Error, TEXT("KAEGAN_VALIDATION: completion state=%d controller=%s"), static_cast<int32>(State), bScreenReady ? TEXT("ready") : TEXT("missing"));
    }
    FGenericPlatformMisc::RequestExit(false);
}

void AKaeganHomeDirector::UpdateProgression()
{
    AKaeganFirstPersonCharacter* Kaegan = GetKaegan();
    if (!Kaegan || State == EKaeganLevelState::Boot || State == EKaeganLevelState::LevelComplete) return;
    const float X = Kaegan->GetActorLocation().X;
    if ((State == EKaeganLevelState::FootstepsActive || State == EKaeganLevelState::MimicRevealed) && X > 3000.f)
    {
        SetState(EKaeganLevelState::HouseCorrupted);
        for (AActor* Prop : CorruptionProps) if (Prop) Prop->SetActorHiddenInGame(false);
        if (Mimic) Mimic->SetRevealed(true);
        if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowNotice(FText::FromString(TEXT("That hallway was not there before.")), 2.6f);
    }
    if (State == EKaeganLevelState::HouseCorrupted && X < 2850.f && !bRetreatObserved)
    {
        bRetreatObserved = true;
        if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowNotice(FText::FromString(TEXT("Behind you, the entrance is gone.")), 2.8f);
    }
    if (State == EKaeganLevelState::HouseCorrupted && X > 3970.f) BeginFinalSequence();
}

void AKaeganHomeDirector::BeginFinalSequence()
{
    if (bFinalStarted) return;
    bFinalStarted = true;
    SetState(EKaeganLevelState::FinalSequence);
    if (AKaeganFirstPersonCharacter* Kaegan = GetKaegan()) Kaegan->SetMovementLocked(true);
    for (APointLight* Light : HouseLights)
    {
        if (UPointLightComponent* Component = Light ? Cast<UPointLightComponent>(Light->GetLightComponent()) : nullptr)
        {
            Component->SetIntensity(0.f);
        }
    }
    if (Mimic) { Mimic->Relocate(FVector(4050, 0, 0)); Mimic->SetRevealed(true); }
    if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowNotice(FText::FromString(TEXT("clink...\n\nclunk.")), 3.2f);
    FTimerHandle CompleteTimer;
    GetWorldTimerManager().SetTimer(CompleteTimer, this, &AKaeganHomeDirector::CompleteLevel, 4.4f, false);
}

void AKaeganHomeDirector::CompleteLevel()
{
    SetState(EKaeganLevelState::LevelComplete);
    if (Mimic) Mimic->SetRevealed(false);
    GetWorldTimerManager().ClearTimer(FootstepTimer);
    if (AKaeganHomePlayerController* PC = Cast<AKaeganHomePlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowCompletion();
}

AKaeganHomeGameMode::AKaeganHomeGameMode()
{
    DefaultPawnClass = AKaeganFirstPersonCharacter::StaticClass();
    PlayerControllerClass = AKaeganHomePlayerController::StaticClass();
}

void AKaeganHomeGameMode::StartPlay()
{
    Super::StartPlay();
    // Deliberately offset from the sofa/television cluster, with a clear sightline to the hall.
    const FVector SafeEntrySpawn(0.f, -300.f, 100.f);
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        AKaeganFirstPersonCharacter* Pawn = Cast<AKaeganFirstPersonCharacter>(PC->GetPawn());
        if (!Pawn)
        {
            Pawn = GetWorld()->SpawnActor<AKaeganFirstPersonCharacter>(SafeEntrySpawn, FRotator::ZeroRotator);
            if (Pawn) PC->Possess(Pawn);
        }
        if (Pawn)
        {
            Pawn->SetActorLocationAndRotation(SafeEntrySpawn, FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
            UE_LOG(LogKaeganHome, Log, TEXT("LEVEL 01 HOME: custom pawn placed at safe entry spawn %s."), *SafeEntrySpawn.ToString());
        }
    }
    bool bDirectorExists = false;
    for (TActorIterator<AKaeganHomeDirector> It(GetWorld()); It; ++It) { bDirectorExists = true; break; }
    if (!bDirectorExists) GetWorld()->SpawnActor<AKaeganHomeDirector>(FVector::ZeroVector, FRotator::ZeroRotator);
}
