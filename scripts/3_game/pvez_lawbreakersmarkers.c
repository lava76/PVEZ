class PVEZ_LawbreakerMarker : Managed {
	vector Position;
	string Id;
	string Name;

	void PVEZ_LawbreakerMarker(vector pos, string id, string name, bool showName) {
		Position = pos;
		Id = id;
		if (showName) {
			Name = name;
		}
	}
}

class PVEZ_LawbreakersMarkers : Managed {
	autoptr array<ref PVEZ_LawbreakerMarker> markers;

	void PVEZ_LawbreakersMarkers() {
		markers = new array<ref PVEZ_LawbreakerMarker>;
	}

	void Update(EntityAI lbEntity, bool isLawbreaker) {
		if (isLawbreaker)
			AddMarker(lbEntity);
		else {
			string id = PVEZ_StaticFunctions.GetEntityId(lbEntity);
			RemoveMarker(id);
		}
		Sync();
	}

	private void AddMarker(EntityAI lbEntity) {
		string id = PVEZ_StaticFunctions.GetEntityId(lbEntity);
		
		bool markerFound = false;
		for (int i = 0; i < markers.Count(); i++) {
			if (markers[i].Id == id) {
				markerFound = true;
				markers[i].Position = lbEntity.GetPosition();
				break;
			}
		}
		if (!markerFound) {
			string name = PVEZ_StaticFunctions.GetEntityName(lbEntity);
			PVEZ_LawbreakerMarker newMarker = new PVEZ_LawbreakerMarker(
				lbEntity.GetPosition(), id, name, g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Name);
			markers.Insert(newMarker);
		}
	}

	void RemoveMarker(string id) {
		for (int i = 0; i < markers.Count(); i++) {
			if (markers[i].Id == id) {
				markers.Remove(i);
				break;
			}
		}
	}

	void UpdateMarkersPositions() {
		if (markers.Count() == 0)
			return;
		
		foreach (PVEZ_LawbreakerMarker marker : markers) {
			EntityAI lbEntity = g_Game.pvez_LawbreakersRoster.GetEntity(marker.Id);
			if (lbEntity) {
				float x = lbEntity.GetPosition()[0];
				float z = lbEntity.GetPosition()[2];
				if (g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location) {
					// Add approximate offset for the marker position:
					int maxOffset = g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location_Max_Offset;
					x += Math.RandomInt(-maxOffset, maxOffset);
					z += Math.RandomInt(-maxOffset, maxOffset);
				}
				marker.Position = Vector(x, 0, z);
			}
		}
		Sync();
	}

	void Sync(array<ref PVEZ_LawbreakerMarker> newMarkers = NULL) {
		if (GetGame().IsServer()) {
			Param1<array<ref PVEZ_LawbreakerMarker>> markersData = new Param1<array<ref PVEZ_LawbreakerMarker>>(markers);
			g_Game.PVEZ_RPCForAllClients(PVEZ_RPC.UPDATE_LAWBREAKERS_MARKERS, markersData);
		}
		else {
			markers = newMarkers;
			if (markers.Count() > 0) {
				// Delete own marker from array if config doesn't allow a lawbreaker to see his own marker
				if (!g_Game.pvez_Config.MAP.Lawbreakers_Markers.Lawbreakers_Can_See_Their_Own_Markers) {
					string id = GetGame().GetPlayer().GetIdentity().GetId();
					for (int i = 0; i < markers.Count(); i++) {
						if (markers[i].Id == id) {
							markers.Remove(i);
							break;
						}
					}
				}
			}
		}
	}
}