#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "KaeganHome.generated.h"

class UCameraComponent;
class UStaticMeshComponent;
class UTextBlock;
class UButton;
class UVerticalBox;
class UBorder;
class UPointLightComponent;
class USceneComponent;
class AStaticMeshActor;
class APointLight;
class ASkyLight;
class ADirectionalLight;
class APostProcessVolume;

UENUM(BlueprintType)
enum class EKaeganLevelState : uint8
{
    Boot,
    ExploreHouse,
    FootstepsActive,
    HouseCorrupted,
    MimicRevealed,
    FinalSequence,
    LevelComplete
};

UENUM()
enum class EKaeganScreenMode : uint8 { Boot, Pause, Complete, Notice };

UCLASS()
class KAEGANEXE_API UKaeganHomeScreen : public UUserWidget
{
    GENERATED_BODY()
public:
    void Configure(EKaeganScreenMode InMode, const FText& InBody, class AKaeganHomePlayerController* InOwner);
    void SetBody(const FText& InBody);
protected:
    virtual void NativeConstruct() override;
private:
    UFUNCTION() void RestartClicked();
    UFUNCTION() void ResumeClicked();
    UFUNCTION() void MenuClicked();
    UFUNCTION() void ContinueClicked();
    void AddButton(const FText& Label, int32 Action);
    UPROPERTY() TObjectPtr<UTextBlock> BodyText;
    UPROPERTY() TObjectPtr<UVerticalBox> ButtonBox;
    UPROPERTY() TObjectPtr<UBorder> Background;
    UPROPERTY() TObjectPtr<class AKaeganHomePlayerController> OwnerController;
    EKaeganScreenMode Mode = EKaeganScreenMode::Boot;
};

UCLASS()
class KAEGANEXE_API AKaeganInteractable : public AActor
{
    GENERATED_BODY()
public:
    AKaeganInteractable();
    virtual FText GetPrompt() const;
    virtual void Interact(class AKaeganFirstPersonCharacter* Character);
    bool IsAvailable() const { return bAvailable; }
    UStaticMeshComponent* GetMesh() const { return Mesh; }
    void ConfigureInteraction(const FText& InPrompt, bool bInAvailable = true) { PromptText = InPrompt; bAvailable = bInAvailable; }
protected:
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Mesh;
    UPROPERTY(EditAnywhere) FText PromptText;
    UPROPERTY(EditAnywhere) bool bAvailable = true;
};

UCLASS()
class KAEGANEXE_API AKaeganDoorActor : public AKaeganInteractable
{
    GENERATED_BODY()
public:
    AKaeganDoorActor();
    virtual FText GetPrompt() const override;
    virtual void Interact(AKaeganFirstPersonCharacter* Character) override;
    void SetLocked(bool bInLocked);
    void SetOpen(bool bInOpen);
protected:
    virtual void Tick(float DeltaSeconds) override;
private:
    UPROPERTY(VisibleAnywhere) TObjectPtr<USceneComponent> Hinge;
    bool bLocked = false;
    bool bOpen = false;
    bool bMoving = false;
    float CurrentAngle = 0.f;
    float TargetAngle = 0.f;
    float OpenDirection = -88.f;
};

UCLASS()
class KAEGANEXE_API AKaeganMemoryObject : public AKaeganInteractable
{
    GENERATED_BODY()
public:
    virtual FText GetPrompt() const override;
    virtual void Interact(AKaeganFirstPersonCharacter* Character) override;
};

UCLASS()
class KAEGANEXE_API AKaeganFatherMimic : public AActor
{
    GENERATED_BODY()
public:
    AKaeganFatherMimic();
    void SetRevealed(bool bReveal);
    void Relocate(const FVector& Position);
protected:
    UPROPERTY(VisibleAnywhere) TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Body;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Head;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> LeftCrutch;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> RightCrutch;
};

UCLASS()
class KAEGANEXE_API AKaeganFirstPersonCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AKaeganFirstPersonCharacter();
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    void SetMovementLocked(bool bLocked);
    void TryInteract();
    AKaeganInteractable* GetFocusedInteractable() const { return FocusedInteractable.Get(); }
protected:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void StartSprint();
    void StopSprint();
    void UpdateInteraction();
    UPROPERTY(VisibleAnywhere) TObjectPtr<UCameraComponent> Camera;
private:
    TWeakObjectPtr<AKaeganInteractable> FocusedInteractable;
    bool bMovementLocked = false;
    bool bSprinting = false;
    float TraceAccumulator = 0.f;
};

UCLASS()
class KAEGANEXE_API AKaeganHomePlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    void ShowBoot();
    void ShowPause();
    void ShowCompletion();
    void ShowNotice(const FText& Text, float Duration = 2.5f);
    void SetInteractionPrompt(const FText& Text);
    void RestartLevel();
    void ReturnToMenu();
    void ResumeGame();
    void ContinueUnavailable();
    void ClearScreen();
    bool HasActiveScreen() const { return ActiveScreen != nullptr; }
protected:
    void TogglePauseMenu();
private:
    UPROPERTY() TObjectPtr<UKaeganHomeScreen> ActiveScreen;
    FString LastPrompt;
    FTimerHandle NoticeTimer;
};

UCLASS()
class KAEGANEXE_API AKaeganHomeDirector : public AActor
{
    GENERATED_BODY()
public:
    AKaeganHomeDirector();
    virtual void BeginPlay() override;
    void AdvanceFromMemory();
    EKaeganLevelState GetState() const { return State; }
protected:
    void BuildHome();
    void SetState(EKaeganLevelState NewState);
    void FinishBoot();
    void RevealDoorwayFigure();
    void HideDoorwayFigure();
    void BeginFinalSequence();
    void CompleteLevel();
    void UpdateProgression();
    void SpawnFootstepHint();
    void RunAutomatedValidation();
    void ConfigureVisibilityPass();
    void LogVisibilityValidation(const TCHAR* Phase) const;
    void RequestValidationScreenshot(const FString& Name) const;
    void ValidateMemoryInteraction();
    void ValidateHallwayCorruption();
    void ValidateEnding();
    void ValidateCompletion();
    AStaticMeshActor* MakeBox(const FVector& Location, const FVector& Dimensions, const FLinearColor& Color, const FString& Label, bool bCollision = true);
    APointLight* MakeLight(const FVector& Location, const FLinearColor& Color, float Intensity, float Radius, const FString& Label, bool bCastShadows = true);
    AKaeganDoorActor* MakeDoor(const FVector& Location, float Yaw, const FString& Label, bool bLocked = false);
    AKaeganFirstPersonCharacter* GetKaegan() const;
private:
    EKaeganLevelState State = EKaeganLevelState::Boot;
    bool bBuilt = false;
    bool bFinalStarted = false;
    bool bRetreatObserved = false;
    UPROPERTY() TObjectPtr<AKaeganMemoryObject> MemoryObject;
    UPROPERTY() TObjectPtr<AKaeganDoorActor> FinalGate;
    UPROPERTY() TObjectPtr<AKaeganFatherMimic> Mimic;
    UPROPERTY() TArray<TObjectPtr<APointLight>> HouseLights;
    UPROPERTY() TArray<TObjectPtr<AActor>> CorruptionProps;
    UPROPERTY() TObjectPtr<ASkyLight> AmbientSkyLight;
    UPROPERTY() TObjectPtr<ADirectionalLight> Moonlight;
    UPROPERTY() TObjectPtr<APostProcessVolume> VisibilityPostProcess;
    FTimerHandle BootTimer;
    FTimerHandle ProgressTimer;
    FTimerHandle FootstepTimer;
    FTimerHandle ValidationTimer;
    FTimerHandle StartingScreenshotTimer;
    FTimerHandle HallwayScreenshotTimer;
};

UCLASS()
class KAEGANEXE_API AKaeganHomeGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    AKaeganHomeGameMode();
    virtual void StartPlay() override;
};
