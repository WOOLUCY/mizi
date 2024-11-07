#include "Actors/Terminal/TerminalScreenWidget.h"
#include "Kismet/GameplayStatics.h"

void UTerminalScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UTerminalScreenWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTerminalScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(InputButton)
	{
		InputButton->OnClicked.AddDynamic(this, &UTerminalScreenWidget::OnInputButtonClicked);
	}

	if(InputText)
	{
		InputText->OnTextCommitted.AddDynamic(this, &UTerminalScreenWidget::OnInputTextCommitted);
	}
}

void UTerminalScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTerminalScreenWidget::OnInputButtonClicked()
{
	InputText->SetKeyboardFocus();
}

void UTerminalScreenWidget::OnInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

	SplitCommand(Text);

	InputText->SetText(FText());
}

void UTerminalScreenWidget::SplitCommand(FText WholeCommand)
{
	FString String = WholeCommand.ToString();

	if (String.Split(TEXT(" "), &Command, &Argument))
	{
		UE_LOG(LogTemp, Log, TEXT("Command: %s"), *Command);
		UE_LOG(LogTemp, Log, TEXT("Argument: %s"), *Argument);
	}
	else
	{
		// 공백이 없는 경우, 전체 문자열을 명령어로 간주
		Command = String;
		UE_LOG(LogTemp, Log, TEXT("Command: %s"), *Command);
		UE_LOG(LogTemp, Warning, TEXT("Argument가 없습니다."));
	}

	ExecuteCommand();
}

void UTerminalScreenWidget::ExecuteCommand()
{
	// 명령어 문자열을 EConsoleCommand enum으로 변환
	ETerminalCommand CommandEnum = GetCommandEnum(Command);

	// switch 문을 사용하여 명령어 처리
	switch (CommandEnum)
	{
	case ETerminalCommand::HELP:
		OnHelp();
		break;
	case ETerminalCommand::MOONS:
		OnMoons();
		break;
	case ETerminalCommand::STORE:
		OnStore();
		break;
	case ETerminalCommand::BESTIARY:
		OnBestiary();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown command: %s"), *Command);
		break;
	}
}

ETerminalCommand UTerminalScreenWidget::GetCommandEnum(const FString& InString)
{
	static const TMap<FString, ETerminalCommand> CommandMap = {
   {"Help", ETerminalCommand::HELP},
   {"Moons", ETerminalCommand::MOONS},
   {"Store", ETerminalCommand::STORE},
   {"Bestiary", ETerminalCommand::BESTIARY}
	};

	if (const ETerminalCommand* FoundCommand = CommandMap.Find(InString))
	{
		return *FoundCommand;
	}

	return ETerminalCommand::UNKNOWN;
}

void UTerminalScreenWidget::OnHelp()
{
	FText MultiLineText = FText::Format(
		FText::FromString(TEXT("{0}\n{1}\n{2}")),
		FText::FromString(TEXT("hello")),
		FText::FromString(TEXT("world")),
		FText::FromString(TEXT("i wanna go home"))
	);
	OutputText->SetText(MultiLineText);
	UE_LOG(LogTemp, Warning, TEXT("OnHelp"));
}

void UTerminalScreenWidget::OnMoons()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMoons"));
}

void UTerminalScreenWidget::OnStore()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStore"));
}

void UTerminalScreenWidget::OnBestiary()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBestiary"));

}
