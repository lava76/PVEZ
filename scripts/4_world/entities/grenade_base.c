modded class Grenade_Base
{
	// In ActionUnpin class when a player unpins a grenade, his entity will be assigned to the grenade entity
    PlayerBase Thrower;

    void SetThrower(PlayerBase thrower)
    {
        Thrower = thrower;
    }
}