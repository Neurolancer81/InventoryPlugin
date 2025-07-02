#include "Items/INV_ItemTags.h"

namespace GameItems
{
	namespace Equipment
	{
		namespace Weapons
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Axe, "GameItems.Equipment.Weapons.Axe", "Axe")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Sword, "GameItems.Equipment.Weapons.Sword", "Sword")
			
		}

		namespace Cloaks
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(RedCloak, "GameItems.Equipment.Cloaks.RedCloak", "Red Cloak")
		}

		namespace Masks
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(SteelMask, "GameItems.Equipment.Masks.SteelMask", "Steel Mask")
		}
	}

	namespace Consumables
	{
		namespace Potions
		{
			namespace Red
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Small, "GameItems.Consumables.Potion.Red.Small", "Small Red Potion")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Large, "GameItems.Consumables.Potion.Red.Large", "Large Red Potion")
			}

			namespace Blue
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Small, "GameItems.Consumables.Potion.Blue.Small", "Small Blue Potion")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Large, "GameItems.Consumables.Potion.Blue.Large", "Large Blue Potion")
			}
		}
	}

	namespace Craftables
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FireFernFruit, "GameItems.Craftables.FireFernFruit", "Fire Fern Fruit")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(LuminDaisy, "GameItems.Craftables.LuminDaisy", "Lumin Daisy")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ScorchPetalBlossom, "GameItems.Craftables.ScorchPetalBlossom", "Scorch Petal Blossom")
				
	}
}
