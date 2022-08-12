#include "PMod.h"
#include "Utilities/MinHook.h"

BPFUNCTION(WriteToFile)
{
	std::cout << "WriteToFile" << std::endl;
	struct InputParams
	{
		UE4::FString NameTest;
		
	};
	auto Inputs = stack->GetInputParams<InputParams>();
	stack->SetOutput<UE4::FString>("OutPutString", L"KboyGang");
	stack->SetOutput<bool>("ReturnValue", true);
}


BPFUNCTION(EnableConsole)
{	
	struct InputParams
	{
		
	};
	if (ZF::UObject::GObjects == nullptr || ZF::FName::GNames == nullptr)
	{
		stack->SetOutput<UE4::FString>("OutPutString", L"Can't find GObjects or GNames");
		return;
		
	}
	else 
	{
		//std::cout << "Enable Consol Called" << std::endl;

		auto GameEngine = ZF::UObject::FindObject<ZF::UGameEngine>("GameEngine Transient.GameEngine_1");
		//	GameEngine->GameViewport->ViewportConsole 
		if (GameEngine == nullptr)
		{	
			Log::Print("[PMOD] GameEngine not found");
			return;
			
		}
		if (GameEngine->ConsoleClass == nullptr)
		{
			Log::Print("[PMOD] ConsoleClass not found");
			return;
		}
		if (GameEngine->GameViewport->ViewportConsole != nullptr)
		{
			Log::Print("[PMOD] Console is already constructed");
			return;
		}

		ZF::UConsole* Console = reinterpret_cast<ZF::UConsole*>(ZF::UObject::StaticConstructObject_Internal(
			GameEngine->ConsoleClass,
			GameEngine->GameViewport,
			"",
			0,
			ZF::FUObjectItem::EInternalObjectFlags::None,
			nullptr,
			false,
			nullptr,
			false
		));
		if (Console == nullptr)
		{
			Log::Print("Coudln't Construct Console");
			return;
		}
		GameEngine->GameViewport->ViewportConsole = Console;
	
	}
	//ZF::APlayerController
	
	
	
}


BPFUNCTION(CPPIncreaseMaxPlayers)
{
	struct InputParams
	{
		

	};
	Log::Print("CPPIncreaseMaxPlayers CALLED");
	for (int i = 0; i < ZF::UObject::GetGlobalObjects().Num(); ++i)
	{
		auto object = ZF::UObject::GetGlobalObjects().GetByIndex(i).Object;

		if (object == nullptr)
		{
			continue;
		}
		
		if (object->IsA(ZF::ABP_GM_HordeMode_C::StaticClass()))
		{	
			auto FullName = object->GetFullName();
			auto token1 = FullName.substr(FullName.find('.') + 1, strlen(FullName.c_str()));
			bool bIsDefaultObj = (token1.rfind("Default__", 0) == 0);
			if (FullName != "BP_GM_HordeMode_C BP_GM_HordeMode.Default__BP_GM_HordeMode_C" && !bIsDefaultObj)
			{	
				

				auto GM_Horde = static_cast<ZF::ABP_GM_HordeMode_C*>(object);

				
				*reinterpret_cast<int*>((DWORD64)(GM_Horde)+0x0460) = 10;
				int Max_Players = *reinterpret_cast<int*>((DWORD64)(GM_Horde)+0x0460);
				Log::Print("Max_Players: "+std::to_string(Max_Players));

				break;
			}
			
		}


	}

}
// Only Called Once, if you need to hook shit, declare some global non changing values
void PMod::InitializeMod()
{	

	//UE4::InitSDK();

	ZF::InitSDK();
	SetupHooks();
	
	REGISTER_FUNCTION(WriteToFile);
	REGISTER_FUNCTION(EnableConsole);
	//REGISTER_FUNCTION(IncreaseMaxPlayers);
	REGISTER_FUNCTION(CPPIncreaseMaxPlayers);
	//MinHook::Init(); //Uncomment if you plan to do hooks
	//MinHook::Add(addr,)

	//UseMenuButton = true; // Allows Mod Loader To Show Button
}


void PMod::InitGameState()
{
}

void PMod::BeginPlay(UE4::AActor* Actor)
{
}

void PMod::PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor)
{
	// Filters Out All Mod Actors Not Related To Your Mod
	std::wstring TmpModName(ModName.begin(), ModName.end());
	if (ModActorName == TmpModName)
	{
		//Sets ModActor Ref
		ModActor = Actor;
	}
}

void PMod::DX11Present(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView)
{
}

void PMod::OnModMenuButtonPressed()
{
}

void PMod::DrawImGui()
{
}