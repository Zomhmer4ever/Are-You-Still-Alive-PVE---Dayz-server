class HordeMission extends SurvivorMissions
{
	//Mission related entities 
	ItemBase MissionObject;
	
	//Mission parameters
	int MsgDlyFinish = 300;					//seconds, message delay time after player has finished mission
	int HordeDensity = 60;					//infected, spawned in circle 
	
	//Mission containers
	ref array<string> InfectedTypes = new array<string>;

	//Mission variables 
			
	
	bool IsExtended() return false;
	
	void HordeMission()
	{
		//Mission mission timeout
		m_MissionTimeout = 2700;			//seconds, mission duration time
		
		//Mission zones
		m_MissionZoneOuterRadius = 200.0;	//meters (!Do not set lower than 200m), mission activation distance
		m_MissionZoneInnerRadius = 3.0;		//meters (!Do not set outside of 1-5m), mission finishing distance to target object
				
		//Mission informant
		m_MissionInformant = "Dr. Legasov";
		
		//Set mission messages
		m_MissionMessage1 = "I got an emergency call from a heli pilot who crashed somewhere in the forests. He is injured but still alive. He was afraid that the infected will find him soon.";
		m_MissionMessage2 = "He maid it to a house in a village near the crash site. He is well equiped and he think he will stay in the house over the night where he treats his open wounds.";
		m_MissionMessage3 = "The house is at the ** "+ m_MissionLocationDir +" side of "+ m_MissionLocation +" **\nBe careful there may be infected and other survivors who want his equipment!";
		

		//Infected types
		//Male												//Female
		InfectedTypes.Insert("ZmbM_CitizenASkinny_Brown");	InfectedTypes.Insert("ZmbF_JournalistNormal_White");
		InfectedTypes.Insert("ZmbM_priestPopSkinny");		InfectedTypes.Insert("ZmbF_Clerk_Normal_White");
		InfectedTypes.Insert("ZmbM_HermitSkinny_Beige");	InfectedTypes.Insert("ZmbF_CitizenANormal_Blue");
		InfectedTypes.Insert("ZmbM_CitizenBFat_Red");		InfectedTypes.Insert("ZmbF_CitizenBSkinny");
		InfectedTypes.Insert("ZmbM_FishermanOld_Green");	InfectedTypes.Insert("ZmbF_HikerSkinny_Grey");
		InfectedTypes.Insert("ZmbM_HunterOld_Autumn");		InfectedTypes.Insert("ZmbF_SurvivorNormal_Orange");
		InfectedTypes.Insert("ZmbM_CitizenBFat_Blue");		InfectedTypes.Insert("ZmbF_HikerSkinny_Green");
		InfectedTypes.Insert("ZmbM_MotobikerFat_Black");	InfectedTypes.Insert("ZmbF_JoggerSkinny_Green");
		InfectedTypes.Insert("ZmbM_JoggerSkinny_Red");		InfectedTypes.Insert("ZmbF_SkaterYoung_Striped");
		InfectedTypes.Insert("ZmbM_MechanicSkinny_Grey");	InfectedTypes.Insert("ZmbF_BlueCollarFat_Red");
		InfectedTypes.Insert("ZmbM_HandymanNormal_Green");	InfectedTypes.Insert("ZmbF_MechanicNormal_Beige");
		InfectedTypes.Insert("ZmbM_HeavyIndustryWorker");	InfectedTypes.Insert("ZmbF_PatientOld");
		InfectedTypes.Insert("ZmbM_Jacket_black");			InfectedTypes.Insert("ZmbF_ShortSkirt_beige");
		InfectedTypes.Insert("ZmbM_Jacket_stripes");		InfectedTypes.Insert("ZmbF_VillagerOld_Red");
		InfectedTypes.Insert("ZmbM_HikerSkinny_Blue");		InfectedTypes.Insert("ZmbF_JoggerSkinny_Red");
		InfectedTypes.Insert("ZmbM_HikerSkinny_Yellow");	InfectedTypes.Insert("ZmbF_MilkMaidOld_Beige");
		InfectedTypes.Insert("ZmbM_PolicemanFat");			InfectedTypes.Insert("ZmbF_VillagerOld_Green");
		InfectedTypes.Insert("ZmbM_PatrolNormal_Summer");	InfectedTypes.Insert("ZmbF_ShortSkirt_yellow");
		InfectedTypes.Insert("ZmbM_JoggerSkinny_Blue");		InfectedTypes.Insert("ZmbF_NurseFat");
		InfectedTypes.Insert("ZmbM_VillagerOld_White");		InfectedTypes.Insert("ZmbF_PoliceWomanNormal");
		InfectedTypes.Insert("ZmbM_SkaterYoung_Brown");		InfectedTypes.Insert("ZmbF_HikerSkinny_Blue");
		InfectedTypes.Insert("ZmbM_MechanicSkinny_Green");	InfectedTypes.Insert("ZmbF_ParamedicNormal_Green");
		InfectedTypes.Insert("ZmbM_DoctorFat");				InfectedTypes.Insert("ZmbF_JournalistNormal_Red");
		InfectedTypes.Insert("ZmbM_PatientSkinny");			InfectedTypes.Insert("ZmbF_SurvivorNormal_White");
		InfectedTypes.Insert("ZmbM_ClerkFat_Brown");		InfectedTypes.Insert("ZmbF_JoggerSkinny_Brown");
		InfectedTypes.Insert("ZmbM_ClerkFat_White");		InfectedTypes.Insert("ZmbF_MechanicNormal_Grey");
		InfectedTypes.Insert("ZmbM_Jacket_magenta");		InfectedTypes.Insert("ZmbF_BlueCollarFat_Green");
		InfectedTypes.Insert("ZmbM_PolicemanSpecForce");	InfectedTypes.Insert("ZmbF_DoctorSkinny");
	}
	
	void ~HordeMission()
	{
		//Despawn all remaining mission objects
		if ( m_MissionObjects )
		{	
			Print("[SMM] Despawning "+ m_MissionObjects.Count() +" mission objects from old mission...");				
			for ( int i = 0; i < m_MissionObjects.Count(); i++ )
			{ 
				if ( !m_MissionObjects.Get(i) ) continue;
				else
				{
					//Delete Object serverside
					GetGame().ObjectDelete( m_MissionObjects.Get(i) );
				}		
			}
			m_MissionObjects.Clear();
		}
		
		//Despawn mission AI's
		if ( m_MissionAIs )
		{
			if ( m_MissionAIs.Count() > 0 )
			{
				Print("[SMM] Despawning "+ m_MissionAIs.Count() +" mission AI's from old mission...");
				for ( int k = 0; k < m_MissionAIs.Count(); k++ )
				{
					GetGame().ObjectDelete( m_MissionAIs.Get(k) );
				}
				m_MissionAIs.Clear();			
			}
			else Print("[SMM] No mission AI's to despawn.");
		}
		
		//Delete PlayersInZone list & reset container takeaway
		if ( m_PlayersInZone )	m_PlayersInZone.Clear();
		if ( m_ContainerWasTaken ) m_ContainerWasTaken = false;
	}
	
	void SpawnObjects()
	{				
		//Mission object is pilot bag
		MissionObject = ItemBase.Cast( GetGame().CreateObject("AliceBag_Camo", m_MissionPosition ) );
		MissionObject.SetPosition( m_MissionPosition + "0 0.2 0" );
		//MissionObject.PlaceOnSurface(); 
		
		//Get random loadout 			
		int selectedLoadout = Math.RandomIntInclusive(0,11);	//!Change randomization limit after adding new loadouts!
		
		//Spawn selected loadout items in mission object
		EntityAI weapon;
								
		if (selectedLoadout == 0)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("M4A1_Green");
					weapon.GetInventory().CreateAttachment("M4_RISHndgrd_Green");
					weapon.GetInventory().CreateAttachment("M4_MPBttstck");
					weapon.GetInventory().CreateAttachment("ACOGOptic");
					weapon.GetInventory().CreateAttachment("M4_Suppressor");
				MissionObject.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
				MissionObject.GetInventory().CreateInInventory("M4_T3NRDSOptic");
				MissionObject.GetInventory().CreateInInventory("Ammo_556x45");
				MissionObject.GetInventory().CreateInInventory("Ammo_556x45");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("Canteen");
				MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			}
			if (selectedLoadout == 1)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("SVD");
					weapon.GetInventory().CreateAttachment("PSO1Optic");
				MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
				MissionObject.GetInventory().CreateInInventory("PSO1Optic");
				MissionObject.GetInventory().CreateInInventory("KazuarOptic");
				MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
				MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
				MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("PeachesCan");
				MissionObject.GetInventory().CreateInInventory("Canteen");
				MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			}
			if (selectedLoadout == 2)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("A2M16A4");
				MissionObject.GetInventory().CreateInInventory("A2M16A4_Magazine");
				MissionObject.GetInventory().CreateInInventory("A2M16A4_Magazine");
				MissionObject.GetInventory().CreateInInventory("Ammo_556x45");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("Ammo_556x45");
				MissionObject.GetInventory().CreateInInventory("PeachesCan");
				MissionObject.GetInventory().CreateInInventory("Canteen");
				MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			}
			if (selectedLoadout == 3)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("Expansion_G36");
				MissionObject.GetInventory().CreateInInventory("Mag_Expansion_G36_30Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_Expansion_G36_30Rnd");
				MissionObject.GetInventory().CreateInInventory("FNX45");
				MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
				MissionObject.GetInventory().CreateInInventory("Ammo_45ACP");
				MissionObject.GetInventory().CreateInInventory("FNP45_MRDSOptic");
				MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
				MissionObject.GetInventory().CreateInInventory("PsilocybeMushroom");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			}	
			if (selectedLoadout == 4)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("A2M40A3");
				MissionObject.GetInventory().CreateInInventory("A2M40A3_Magazine");
				MissionObject.GetInventory().CreateInInventory("A2M40A3_Magazine");
				MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
				MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
				weapon = MissionObject.GetInventory().CreateInInventory("FNX45");
					weapon.GetInventory().CreateAttachment("PistolSuppressor");
					EntityAI weaponlight = weapon.GetInventory().CreateAttachment("TLRLight");
						weaponlight.GetInventory().CreateAttachment("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
				MissionObject.GetInventory().CreateInInventory("Ammo_45ACP");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
			}	
			if (selectedLoadout == 5)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("A2mk17");
				MissionObject.GetInventory().CreateInInventory("A2mk17_mag");
				MissionObject.GetInventory().CreateInInventory("A2mk17_mag");
				MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
				MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
				MissionObject.GetInventory().CreateInInventory("FNX45");
				MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
				MissionObject.GetInventory().CreateInInventory("Ammo_45ACP");
				MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
				MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
				MissionObject.GetInventory().CreateInInventory("TLRLight");
				MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			}
		if (selectedLoadout == 6)
		{			
			weapon = MissionObject.GetInventory().CreateInInventory("SNAFUMagnum500");
			MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
			MissionObject.GetInventory().CreateInInventory("SNAFU_Ammo_500_SW");
			MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
			MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
			MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");	
			MissionObject.GetInventory().CreateInInventory("SpaghettiCan");
			MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");				
		}
		if (selectedLoadout == 7)
		{			
			weapon = MissionObject.GetInventory().CreateInInventory("AK74");
				weapon.GetInventory().CreateAttachment("AK_RailHndgrd");
				weapon.GetInventory().CreateAttachment("AK74_WoodBttstck");	
				weapon.GetInventory().CreateAttachment("KashtanOptic");
				weapon.GetInventory().CreateAttachment("'AK_Suppressor");			
			MissionObject.GetInventory().CreateInInventory("Mag_AK74_30Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_AK74_30Rnd");
			MissionObject.GetInventory().CreateInInventory("Headtorch_Grey");
			MissionObject.GetInventory().CreateInInventory("NVGoggles");
			MissionObject.GetInventory().CreateInInventory("Canteen");	
			MissionObject.GetInventory().CreateInInventory("TacticalBaconCan");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");				
		}
		if (selectedLoadout == 8)
		{			
			weapon = MissionObject.GetInventory().CreateInInventory("A2HKM5_SD");			
			MissionObject.GetInventory().CreateInInventory("A2HKM5_SD_Magazine");
			MissionObject.GetInventory().CreateInInventory("A2HKM5_SD_Magazine");
			MissionObject.GetInventory().CreateInInventory("M67Grenade");
			MissionObject.GetInventory().CreateInInventory("M67Grenade");
			MissionObject.GetInventory().CreateInInventory("Zen_ArtillerySmokeGrenade");
			MissionObject.GetInventory().CreateInInventory("Canteen");	
			MissionObject.GetInventory().CreateInInventory("PortableGasStove");
			MissionObject.GetInventory().CreateInInventory("SmallGasCanister");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");			
		}
		if (selectedLoadout == 9)
		{			
			weapon = MissionObject.GetInventory().CreateInInventory("A2MicroUzi");		
			MissionObject.GetInventory().CreateInInventory("A2UZI_Magazine");
				MissionObject.GetInventory().CreateInInventory("A2UZI_Magazine");
			MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
			MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
			MissionObject.GetInventory().CreateInInventory("ExpansionGoldBar");
			MissionObject.GetInventory().CreateInInventory("CombatKnife");
			MissionObject.GetInventory().CreateInInventory("FieldShovel");
			MissionObject.GetInventory().CreateInInventory("Canteen");	
			MissionObject.GetInventory().CreateInInventory("FS_Buter");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");
			MissionObject.GetInventory().CreateInInventory("MC_Battery9V");			
		}
						
		Print("[SMM] Mission rewards spawned in reward container. Randomly selected loadout was "+ selectedLoadout +"." );
		
		m_MissionObjects.Insert( MissionObject );
		
		Print("[SMM] Survivor Mission "+ m_selectedMission +" :: "+ m_MissionName +" ...mission deployed!");
	}
	
	void SpawnAIs()
	{	
		//Spawn pilot
		DayZInfected InfectedPilot = DayZInfected.Cast( GetGame().CreateObject( "ZmbM_CommercialPilotOld_Olive" , m_MissionPosition , false , true ));
			InfectedPilot.SetPosition( m_MissionPosition );
		//InfectedPilot.PlaceOnSurface();
			InfectedPilot.GetInventory().CreateAttachment("ZSh3PilotHelmet_Green");
			InfectedPilot.GetInventory().CreateInInventory("M67Grenade");
			InfectedPilot.GetInventory().CreateInInventory("CanOpener");
			InfectedPilot.GetInventory().CreateInInventory("BandageDressing");
			InfectedPilot.GetInventory().CreateInInventory("PeachesCan");
			InfectedPilot.GetInventory().CreateInInventory("Canteen");
			InfectedPilot.GetInventory().CreateAttachment("UKAssVest_Olive");
			InfectedPilot.GetInventory().CreateInInventory("PersonalRadio");
			InfectedPilot.GetInventory().CreateInInventory("Battery9V");
			InfectedPilot.GetInventory().CreateInInventory("Battery9V");
			InfectedPilot.GetInventory().CreateInInventory("Battery9V");
			
		//Pilot is dead
		InfectedPilot.SetHealth("","",0);
		
		m_MissionAIs.Insert( InfectedPilot );
		
		//Spawn horde
		for ( int j = 0; j < HordeDensity; j++ )
		{
			//calc new spawn position
			float x = Math.RandomFloatInclusive( 3 , 30 );
			float y = Math.RandomFloatInclusive( 3 , 30 );	
			int Dice = Math.RandomIntInclusive( 0, 9);
			if ( Dice > 4 ) x *= -1.0;
			Dice = Math.RandomIntInclusive( 0, 9);
			if ( Dice < 5 ) y *= -1.0;
			vector NewPosVector = { x, 0, y };
			
			//Spawn infected
			m_MissionAIs.Insert( GetGame().CreateObject( InfectedTypes.GetRandomElement(), m_MissionPosition + NewPosVector, false, true ));	
		}
	}
	
	void ObjDespawn() 
	{	
		//nothing to despawn
	}
	
	void MissionFinal()
	{	//When player enters last mission target zone	

		//Alert infected 
		for ( int j = 1 ; j < m_MissionAIs.Count(); j++ )
		{
			DayZInfected Zed = DayZInfected.Cast( m_MissionAIs.Get(j));
			if ( Zed )
			{
				DayZInfectedCommandMove moveCommandZ = Zed.GetCommand_Move();
				moveCommandZ.SetIdleState(2);
			}
		}
		
		//Finish mission
		m_RewardsSpawned = true;
		m_MsgNum = -1;
		m_MsgChkTime = m_MissionTime + MsgDlyFinish;			
	}
	
	void PlayerChecks( PlayerBase player )
	{
		
	}
		
	void UpdateBots(float dt)
	{
			
	}
	
	bool DeployMission()
	{	//When first player enters the mission zone (primary/secondary)
		if ( m_MissionPosition && m_MissionPosition != "0 0 0" )
		{
			//Call spawners	
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( this.SpawnObjects );
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.SpawnAIs, 1000, false );
			return true;		
		}
		else 
		{
			Print("[SMM] ERROR : Mission position was rejected or doesn't exist. MissionPosition is NULL!");
			return false;
		}
	}
}

