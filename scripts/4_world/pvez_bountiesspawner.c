class PVEZ_BountiesSpawner : Managed {

	PlayerBase player;

	void PVEZ_BountiesSpawner(PlayerBase p) {
		player = p;
	}

	void SpawnRewards(array<ref PVEZ_BountyItemData> rewards, string victimName) {
		// Notifiction message "For killing the lawbreaker <name> you're rewwarded with"
		string msg = "#pvez_bnty_msg1" + " " + victimName + " " + "#pvez_bnty_msg2";

		// To add "Some items are dropped on the ground" to the messsage, if inventory spawn failed
		bool failedToPutInInventory;

		PVEZ_BountyItemData bntItemData;
		for (int i = 0; i < rewards.Count(); i++) {
			bntItemData = rewards[i];
			// Add reward name and quantity into the message
			msg = msg + "\n" + bntItemData.displayName + " x" + bntItemData.amount;
			
			// This will store amount of items that wasn't spawned due to no free space left in the inventory.
			int amountNotGiven = 0;
			PutInInventory(bntItemData.className, bntItemData.amount, amountNotGiven);
			if (amountNotGiven > 0) {
				failedToPutInInventory = true;
				PutOnGround(bntItemData.className, amountNotGiven);
			}
		}
		if (failedToPutInInventory)
			msg = msg + "\n" + "#pvez_bnty_msg3"; // "... Some items are dropped on the ground"
		PVEZ_Notifications.PersonalNotification(player, PVEZ_NotificationType.NOTIF_GENERIC, 15, false, msg);
	}

	// PutInInventory will try to create an item in inventory, set stack quantity to the amount from reward.amount,
	// and if the amount is more the item's max stack size, the method will set the out int amountNotGiven to (amountToSpawn - maxQuantity)
	// and start itself again to create a new stack.
	void PutInInventory(string className, int amountToGive, out int amountNotGiven) {
		ItemBase item = ItemBase.Cast(player.GetInventory().CreateInInventory(className));
		// if there was a free space in inventory
		if (item) {
			FillStack(item, amountToGive, amountNotGiven);
			if (amountNotGiven > 0) {
				// Items didn't fit into the stack, spawn another stack
				PutInInventory(className, amountNotGiven, amountNotGiven);
			}
		}
		// if no free space
		else {
			amountNotGiven = amountToGive;
		}
	}

	void PutOnGround(string className, int amountToGive) {
		ItemBase item = ItemBase.Cast(player.SpawnEntityOnGroundPos(className, player.GetPosition()));
		int amountNotGiven = 0;
		if (item) {
			FillStack(item, amountToGive, amountNotGiven);
			if (amountNotGiven > 0)
				// Items didn't fit into the stack, spawn another stack
				PutOnGround(className, amountNotGiven);
		}
	}

	void FillStack(ItemBase item, int amountToGive, out int amountNotGiven) {
		int maxQnt = item.GetQuantityMax();
		// Some mod items don't have neither "varStackMax" nor "varQuantityMax" in there class config,
		// and item.GetQuantityMax() will return 0 in this case. Set it to 1 then.
		if (maxQnt == 0) maxQnt = 1;
		if (amountToGive <= maxQnt) {
			item.SetQuantity(amountToGive);
			amountNotGiven = 0;
		}
		else {
			item.SetQuantity(maxQnt);
			amountNotGiven = amountToGive - maxQnt;
		}
	}
}