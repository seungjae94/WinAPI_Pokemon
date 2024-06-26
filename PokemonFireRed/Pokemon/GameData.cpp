﻿#include "GameData.h"
#include <EngineBase/EngineDirectory.h>
#include "CsvReader.h"

std::map<std::string, int> UGameData::NameResolver;

std::map<EPokemonId, FPokemonSpecies> UGameData::Species;
std::map<EPokemonMove, FPokemonMove> UGameData::Moves;
std::map<EPokemonNature, FPokemonNature> UGameData::Natures;
std::map<EPokemonAbility, FPokemonAbility> UGameData::Abilities;
std::map<EPokemonGender, FPokemonGender> UGameData::Genders;
std::map<EPokemonStatus, FPokemonStatus> UGameData::Statuses;
std::map<EPokemonType, FPokemonType> UGameData::Types;
std::list<EPokemonId> UGameData::ImplementedSpeciesNo;

std::map<std::string, std::map<int, UWildPokemonZone>> UGameData::WildPokemonZones;

std::map<EItemId, FItem> UGameData::Items;

class GameDataInitiator
{
public:
	GameDataInitiator()
	{
		CurDir.MoveToSearchChild("Resources");
		CurDir.Move("GameData");
		InitNameResolver();
		GeneratePokemons();
		GenerateMoves();
		GenerateAbilities();
		GenerateNatures();
		GenerateTypes();
		GenerateStatuses();
		GenerateGenders();
		GenerateWildPokemonZones();
		GenerateItems();
	}

	void InitNameResolver()
	{
		UGameData::NameResolver[""] = 0;

		std::string FilePath = CurDir.AppendPath("NameResolver.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			int IntValue = std::stoi(Line[0]);
			for (int i = 1; i < Line.size(); ++i)
			{
				if (false == Line[i].empty())
				{
					std::string UpperLine = UEngineString::ToUpper(Line[i]);
					UGameData::NameResolver[UpperLine] = IntValue;
					UGameData::NameResolver[UPokemonString::RemoveSpace(UpperLine)] = IntValue;
				}
			}
		}
	}

	void GeneratePokemons() {
		// 레벨업 스킬 추가
		std::map<EPokemonId, std::map<int, std::vector<EPokemonMove>>> LevelUpMoveMap;

		std::string LevelUpMoveFilePath = CurDir.AppendPath("LevelUpMove.csv");
		UCsvReader LevelUpMoveFileReader = UCsvReader(LevelUpMoveFilePath);
		std::vector<std::vector<std::string>> LevelUpMoveLines = LevelUpMoveFileReader.ReadLines();

		for (std::vector<std::string>& Line : LevelUpMoveLines)
		{
			EPokemonId Id = static_cast<EPokemonId>(UGameData::Resolve(Line[0]));
			int Level = std::stoi(Line[1]);
			EPokemonMove MoveId = static_cast<EPokemonMove>(UGameData::Resolve(Line[2]));

			LevelUpMoveMap[Id][Level].push_back(MoveId);
		}

		// 포켓몬 종족 추가
		std::string PokemonFilePath = CurDir.AppendPath("Pokemon.csv");
		UCsvReader PokemonFileReader = UCsvReader(PokemonFilePath);
		std::vector<std::vector<std::string>> PokemonLines = PokemonFileReader.ReadLines();

		for (std::vector<std::string>& Line : PokemonLines)
		{
			FPokemonSpecies Species = FPokemonSpecies();
			Species.Id = static_cast<EPokemonId>(std::stoi(Line[0]));
			Species.Name = Line[1];
			Species.BHp = std::stoi(Line[2]);
			Species.BAtk = std::stoi(Line[3]);
			Species.BDef = std::stoi(Line[4]);
			Species.BSpAtk = std::stoi(Line[5]);
			Species.BSpDef = std::stoi(Line[6]);
			Species.BSpeed = std::stoi(Line[7]);
			Species.YHp = std::stoi(Line[8]);
			Species.YAtk = std::stoi(Line[9]);
			Species.YDef = std::stoi(Line[10]);
			Species.YSpAtk = std::stoi(Line[11]);
			Species.YSpDef = std::stoi(Line[12]);
			Species.YSpeed = std::stoi(Line[13]);
			Species.YExp = std::stoi(Line[14]);
			Species.ExpGroup = static_cast<EExperienceGroup>(UGameData::Resolve(Line[15]));
			Species.MaleRatio = std::stof(Line[16]);
			Species.CatchRate = std::stoi(Line[17]);
			Species.Friendship = std::stoi(Line[18]);

			for (int i = 19; i <= 20; ++i)
			{
				int LineInt = UGameData::Resolve(Line[i]);

				if (0 == LineInt)
				{
					continue;
				}

				Species.TypeIds.push_back(static_cast<EPokemonType>(LineInt));
			}

			for (int i = 21; i <= 22; ++i)
			{
				int LineInt = UGameData::Resolve(Line[i]);

				if (0 == LineInt)
				{
					continue;
				}

				Species.AbilityCandidateIds.push_back(static_cast<EPokemonAbility>(LineInt));
			}

			for (const std::pair<int, std::vector<EPokemonMove>>& Pair : LevelUpMoveMap[Species.Id])
			{
				int Level = Pair.first;
				for (EPokemonMove MoveId : Pair.second)
				{
					Species.AddLevelUpMove(Level, MoveId);
				}
			}

			UGameData::Species[Species.Id] = Species;
			UGameData::ImplementedSpeciesNo.push_back(Species.Id);
		}
	}
	void GenerateMoves() {
		std::string FilePath = CurDir.AppendPath("Move.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonMove Move = FPokemonMove();
			Move.Id = static_cast<EPokemonMove>(std::stoi(Line[0]));
			Move.Name = Line[1];
			Move.Explain = Line[2];
			Move.TypeId = static_cast<EPokemonType>(UGameData::Resolve(Line[3]));
			Move.PP = std::stoi(Line[4]);
			Move.BasePower = std::stoi(Line[5]);
			Move.Accuracy = std::stoi(Line[6]);
			//Move.IsContact = UGameData::Resolve(Line[7]);
			Move.BETarget = static_cast<EMoveEffectTarget>(UGameData::Resolve(Line[8]));
			if (false == Line[9].empty())
			{
				Move.BEStatStageId = static_cast<EStatStageChangeType>(UGameData::Resolve(Line[9]));
			}
			if (false == Line[10].empty())
			{
				Move.BEStatStageValue = std::stoi(Line[10]);
			}
			if (false == Line[11].empty())
			{
				Move.BEStatusId = static_cast<EPokemonStatus>(UGameData::Resolve(Line[11]));
			}

			Move.SETarget = static_cast<EMoveEffectTarget>(UGameData::Resolve(Line[12]));
			if (Line[13] != "")
			{
				Move.SERate = std::stoi(Line[13]);
			}
			if (Line[14] != "")
			{
				Move.SEStatStageId = static_cast<EStatStageChangeType>(UGameData::Resolve(Line[14]));
			}
			if (Line[15] != "")
			{
				Move.SEStatStageValue = std::stoi(Line[15]);
			}
			if (Line[16] != "")
			{
				Move.SEStatusId = static_cast<EPokemonStatus>(UGameData::Resolve(Line[16]));
			}

			UGameData::Moves[Move.Id] = Move;
		}
	}
	void GenerateNatures()
	{
		std::string FilePath = CurDir.AppendPath("Nature.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonNature Nature = FPokemonNature();
			Nature.Id = static_cast<EPokemonNature>(std::stoi(Line[0]));
			Nature.Name = Line[1];
			Nature.NAtk = std::stof(Line[2]);
			Nature.NDef = std::stof(Line[3]);
			Nature.NSpAtk = std::stof(Line[4]);
			Nature.NSpDef = std::stof(Line[5]);
			Nature.NSpeed = std::stof(Line[6]);
			UGameData::Natures[Nature.Id] = Nature;
		}
	}
	void GenerateAbilities()
	{
		std::string FilePath = CurDir.AppendPath("Ability.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonAbility Ability = FPokemonAbility();
			Ability.Id = static_cast<EPokemonAbility>(std::stoi(Line[0]));
			Ability.Name = Line[1];
			Ability.Explain = Line[2];
			UGameData::Abilities[Ability.Id] = Ability;
		}
	}
	void GenerateTypes()
	{
		std::string FilePath = CurDir.AppendPath("Type.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonType Type = FPokemonType();
			Type.Id = static_cast<EPokemonType>(std::stoi(Line[0]));
			Type.Name = Line[1];
			Type.ImageName = Line[2];

			for (int i = 3; i <= 19; ++i)
			{
				EPokemonType EnemyType = static_cast<EPokemonType>(i - 2);
				Type.EffectTo[EnemyType] = std::stof(Line[i]);
			}

			UGameData::Types[Type.Id] = Type;
		}
	}

	void GenerateStatuses()
	{
		std::string FilePath = CurDir.AppendPath("Status.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonStatus Status = FPokemonStatus();
			Status.Id = static_cast<EPokemonStatus>(std::stoi(Line[0]));
			Status.Name = Line[1];
			Status.ImageName = Line[2];

			UGameData::Statuses[Status.Id] = Status;
		}
	}

	void GenerateGenders()
	{
		UGameData::Genders[EPokemonGender::Male] = FPokemonGender(
			EPokemonGender::Male, RN::GenderMarkMale, RN::BigGenderMarkMale
		);
		UGameData::Genders[EPokemonGender::Female] = FPokemonGender(
			EPokemonGender::Female, RN::GenderMarkFemale, RN::BigGenderMarkFemale
		);
	}

	void GenerateWildPokemonZones()
	{
		std::string FilePath = CurDir.AppendPath("WildPokemonZone.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			std::string MapName = UEngineString::ToUpper(Line[0]);
			int ZoneId = std::stoi(Line[1]);

			UWildPokemonZone& Zone = UGameData::WildPokemonZones[MapName][ZoneId];
			FWildPokemonEncounter Encounter;
			Encounter.Id = static_cast<EPokemonId>(UGameData::Resolve(Line[2]));
			Encounter.Prop = std::stof(Line[3]);
			Encounter.MinLevel = std::stoi(Line[4]);
			Encounter.MaxLevel = std::stoi(Line[5]);
			Zone.AddWildPokemonEncounter(Encounter);
		}
	}

	void GenerateItems()
	{
		std::string FilePath = CurDir.AppendPath("Item.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FItem Item = FItem();
			Item.Id = static_cast<EItemId>(std::stoi(Line[0]));
			Item.Type = static_cast<EItemType>(UGameData::Resolve(Line[1]));
			Item.Name = UGameData::AnsiToUnicodeWithReplacement(Line[2]);
			Item.ImageName = Line[3];
			Item.Explain = UGameData::AnsiToUnicodeWithReplacement(Line[4]);
			Item.BuyPrice = std::stoi(Line[5]);

			if (false == Line[6].empty())
			{
				Item.UseEffect = static_cast<EUseEffect>(UGameData::Resolve(Line[6]));
			}

			if (false == Line[7].empty())
			{
				Item.HealValue = std::stoi(Line[7]);
			}

			UGameData::Items[Item.Id] = Item;
		}
	}

	UEngineDirectory CurDir;
};

GameDataInitiator Init;

const FPokemonSpecies* UGameData::FindSpecies(EPokemonId _Id)
{
	if (false == Species.contains(_Id))
	{
		MsgBoxAssert("아직 구현되지 않은 포켓몬을 찾으려고 했습니다. Id: " + static_cast<int>(_Id));
		return nullptr;
	}

	return &Species[_Id];
}

const FPokemonMove* UGameData::FindMove(EPokemonMove _Id)
{
	if (false == Moves.contains(_Id))
	{
		MsgBoxAssert("아직 구현되지 않은 기술을 찾으려고 했습니다. Id: " + static_cast<int>(_Id));
		return nullptr;
	}

	return &Moves[_Id];
}

const FPokemonNature* UGameData::FindNature(EPokemonNature _Id)
{
	if (false == Natures.contains(_Id))
	{
		MsgBoxAssert("아직 구현되지 않은 성격을 찾으려고 했습니다. Id: " + static_cast<int>(_Id));
		return nullptr;
	}

	return &Natures[_Id];
}

const FPokemonAbility* UGameData::FindAbility(EPokemonAbility _Id)
{
	if (false == Abilities.contains(_Id))
	{
		MsgBoxAssert("아직 구현되지 않은 특성을 찾으려고 했습니다. Id: " + static_cast<int>(_Id));
		return nullptr;
	}

	return &Abilities[_Id];
}

const FPokemonGender* UGameData::FindGender(EPokemonGender _Id)
{
	if (false == Genders.contains(_Id))
	{
		return nullptr;
	}

	return &Genders[_Id];
}

const FPokemonType* UGameData::FindType(EPokemonType _Id)
{
	if (false == Types.contains(_Id))
	{
		MsgBoxAssert("아직 구현되지 않은 타입을 찾으려고 했습니다. Id: " + static_cast<int>(_Id));
		return nullptr;
	}

	return &Types[_Id];
}

const FPokemonStatus* UGameData::FindStatus(EPokemonStatus _Id)
{
	if (false == Statuses.contains(_Id))
	{
		MsgBoxAssert("아직 구현되지 않은 상태를 찾으려고 했습니다. Id: " + static_cast<int>(_Id));
		return nullptr;
	}

	return &Statuses[_Id];
}

const UWildPokemonZone* UGameData::FindWildPokemonZone(std::string_view _MapName, int _Index)
{
	std::string MapName = UEngineString::ToUpper(_MapName);

	if (false == WildPokemonZones.contains(MapName) || false == WildPokemonZones[MapName].contains(_Index))
	{
		MsgBoxAssert("아직 구현되지 않은 야생 포켓몬 존을 찾으려고 했습니다. 존 = " + MapName + ":" + std::to_string(_Index));
		return nullptr;
	}

	return &WildPokemonZones[MapName][_Index];
}

const FItem* UGameData::FindItem(EItemId _Id)
{
	if (false == Items.contains(_Id))
	{
		MsgBoxAssert("아직 구현되지 않은 아이템을 찾으려고 했습니다. Id: " + static_cast<int>(_Id));
		return nullptr;
	}

	return &Items[_Id];
}

std::list<EPokemonId> UGameData::GetImplementedSpeciesNo()
{
	return ImplementedSpeciesNo;
}

std::string UGameData::GetSpeciesName(EPokemonId _Id)
{
	return Species[_Id].Name;
}

int UGameData::Resolve(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == NameResolver.contains(UpperName))
	{
		MsgBoxAssert("UGameData::NameResolver에서 에러가 발생했습니다.");
		return -1;
	}

	return NameResolver[UpperName];
}

std::wstring UGameData::AnsiToUnicodeWithReplacement(std::string _Text)
{
	std::wstring Text = UEngineString::AnsiToUniCode(_Text);
	std::replace(Text.begin(), Text.end(), L'*', L'é');
	return Text;
}


