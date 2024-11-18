#include "Actors/Terminal/TerminalScreenWidget.h"

#include "Terminal.h"
#include "Framework/BasicGameMode.h"
#include "Framework/BasicGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "PlayerCharacter/BasicCharacter.h"

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

	SetData(DataTableRowHandle);

	if(InputButton)
	{
		InputButton->OnClicked.AddDynamic(this, &UTerminalScreenWidget::OnInputButtonClicked);
	}

	if(InputText)
	{
		InputText->OnTextCommitted.AddDynamic(this, &UTerminalScreenWidget::OnInputTextCommitted);
	}

	UpdateMoneyText();
}

void UTerminalScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UTerminalScreenWidget::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FCommandTableRow* Data = DataTableRowHandle.GetRow<FCommandTableRow>(TEXT("Help"));
	if (!Data) { ensure(false); return; }

	CommandData = Data;

	OutputText->SetText(CommandData->OutputText);
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
		Command = String;
	}

	ExecuteCommand();
}

void UTerminalScreenWidget::ExecuteCommand()
{
	// ��ɾ� ���ڿ��� EConsoleCommand enum���� ��ȯ
	ETerminalCommand CommandEnum = GetCommandEnum(Command);
	ChangeOutputText(DataTableRowHandle, Command);

	// switch ���� ����Ͽ� ��ɾ� ó��
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
	case ETerminalCommand::ITEM:
		OnPurchaseItem();
		break;
	case ETerminalCommand::MIZI:
		OnMizi();
		break;
	default:
		//ChangeOutputText(DataTableRowHandle, "Unknown");
		break;
	}
}

ETerminalCommand UTerminalScreenWidget::GetCommandEnum(const FString& InString)
{
	static const TMap<FString, ETerminalCommand> CommandMap = {
		{"Help", ETerminalCommand::HELP},
		{"Moons", ETerminalCommand::MOONS},
		{"Store", ETerminalCommand::STORE},
		{"Bestiary", ETerminalCommand::BESTIARY},

		{"Flashlight", ETerminalCommand::ITEM},
		{"Rifle", ETerminalCommand::ITEM},
		{"RifleBullet", ETerminalCommand::ITEM},
		{"Shovel", ETerminalCommand::ITEM},
		{"Grenade", ETerminalCommand::ITEM},


		{"Mizi", ETerminalCommand::MIZI},
			

		{"Unknown", ETerminalCommand::UNKNOWN},
	};

	if (const ETerminalCommand* FoundCommand = CommandMap.Find(InString))
	{
		return *FoundCommand;
	}

	return ETerminalCommand::UNKNOWN;
}

FText UTerminalScreenWidget::ChangeOutputText(const FDataTableRowHandle& InDataTableRowHandle, FString String)
{
	DataTableRowHandle.RowName = FName(*String);

	if (DataTableRowHandle.DataTable == nullptr || DataTableRowHandle.IsNull())
	{
		ensure(false);
		return FText::FromString("");
	}

	FCommandTableRow* NewData = DataTableRowHandle.GetRow<FCommandTableRow>(String);
	if (NewData)
	{
		CommandData = NewData;
		OutputText->SetText(CommandData->OutputText);

		return CommandData->OutputText;
	}

	return FText::FromString("");
}

void UTerminalScreenWidget::OnHelp()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHelp"));
}

void UTerminalScreenWidget::OnMoons()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMoons"));
}

void UTerminalScreenWidget::OnStore()
{
	// TODO: 가격표 표시
	ABasicGameState* GameState = Cast<ABasicGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if(!GameState)
	{
		// 게임스테이트가 올바르지 않음
		ensure(false);
		return;
	}

	// 상점 아이템 목록 가져오기
	const TMap<FString, int32>& StoreMenu = GameState->StoreItemList;

	// MultiLineText에 아이템과 가격을 추가
	FText MultiLineText;
	FText LineBreak = FText::FromString("\n");
	FFormatNamedArguments Args;
	MultiLineText = ChangeOutputText(DataTableRowHandle, Command);
	for (const TPair<FString, int32>& Item : StoreMenu)
	{
		// 아이템 이름과 가격을 문자열로 결합
		FText ItemText = FText::Format(
			FText::FromString("* {0}: {1} Credits"),
			FText::FromString(Item.Key),
			FText::AsNumber(Item.Value)
		);

		// 여러 줄로 표시하기 위해 ItemText를 MultiLineText에 추가
		MultiLineText = FText::Join(LineBreak, MultiLineText, ItemText);
	}

	// OutputText 위젯에 MultiLineText 설정
	if (OutputText)
	{
		OutputText->SetText(MultiLineText);
	}
}

void UTerminalScreenWidget::OnBestiary()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBestiary"));

}

void UTerminalScreenWidget::OnPurchaseItem()
{
	// Afford한지 확인

	ABasicGameState* GameState = Cast<ABasicGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GameState)
	{
		// 게임스테이트가 올바르지 않음
		ensure(false);
		return;
	}

	// 플레이어 스테이트에서 구매한 가격만큼 차감
	ABasicPlayerState* PlayerStatus = Cast<ABasicPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!PlayerStatus)
	{
		// 플레이어 스테이트가 올바르지 않음
		ensure(false);
		return;
	}

	int RepeatNum = UKismetStringLibrary::Conv_StringToInt(Argument);
	if(RepeatNum == 0)
	{
		RepeatNum = 1;
	}
	const TMap<FString, int32>& StoreMenu = GameState->StoreItemList;
	const int32* FoundValue = StoreMenu.Find(Command);
	if (!FoundValue)	// 메뉴에 해당 아이템이 없음
	{
		ensure(false);
		ChangeOutputText(DataTableRowHandle, "Unknown");
		return;
	}

	int32 PaidMoney = (*FoundValue * RepeatNum);
	int32 NewMoney = PlayerStatus->GetCurMoney() - PaidMoney;
	if(NewMoney < 0)
	{
		// 금액이 부족한 상황
		ChangeOutputText(DataTableRowHandle, "Refuse");
		return;
	}

	PlayerStatus->SetCurMoney(NewMoney);

	// 구매 성공
	FText MultiLineText;
	FText LineBreak = FText::FromString("\n");
	FText ItemText = FText::Format(FText::FromString("You paid {0} credits to buy {1}."),
		FText::AsNumber(PaidMoney), FText::FromString(Command));
	MultiLineText = FText::Join(LineBreak, MultiLineText, ItemText);
	ItemText = FText::FromString("Thank you for using store.");
	MultiLineText = FText::Join(LineBreak, MultiLineText, ItemText);

	// OutputText 위젯에 MultiLineText 설정
	if (OutputText)
	{
		OutputText->SetText(MultiLineText);
	}

	// Terminal에서 아이템 스폰
	ATerminal* Terminal = Cast<ATerminal>(UGameplayStatics::GetActorOfClass(GetWorld(), ATerminal::StaticClass()));
	if(!Terminal)
	{
		ensure(false);
		return;
	}
	Terminal->SpawnOrderdItem(Command, Argument);

	UpdateMoneyText();
}

void UTerminalScreenWidget::UpdateMoneyText()
{
	// 돈 금액 업데이트
	ABasicPlayerState* PlayerState = Cast<ABasicPlayerState>(GetOwningPlayerState());
	if (PlayerState)
	{
		FString String = FString::FromInt(PlayerState->GetCurMoney());
		MoneyText->SetText(FText::FromString(String));
	}
}

void UTerminalScreenWidget::OnMizi()
{
	// 레벨 이름은 문자열로 지정합니다 (예: "NewLevel").
	FName LevelName = "FactoryLevel";

	// Level을 전환하며 레벨 옵션을 적용합니다.
	UGameplayStatics::OpenLevel(this, LevelName);
}
