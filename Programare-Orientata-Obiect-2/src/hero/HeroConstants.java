package hero;

public final class HeroConstants {
    // pyromancer constants
    static final int PYROINITHP = 500;
    static final int PYROHPBONUS = 50;

    static final Character PYROTERRAIN = 'V';
    static final float PYROTERRAINBONUS = 1.25F;

    // --> fireblast constants
    static final int FIREBLAST_BASE_DAMAGE = 350;
    static final int FIREBLAST_LVLBONUS_DAMAGE = 50;

    static final float FIREBLAST_ROGUE_MOD = 0.8F;
    static final float FIREBLAST_KNIGHT_MOD = 1.2F;
    static final float FIREBLAST_PYROMANCER_MOD = 0.9F;
    static final float FIREBLAST_WIZARD_MOD = 1.05F;

    // --> ignite constants
    static final int IGNITE_BASE_DAMAGE = 150;
    static final int IGNITE_LVLBONUS_DAMAGE = 30;

    static final float IGNITE_ROGUE_MOD = 0.8F;
    static final float IGNITE_KNIGHT_MOD = 1.2F;
    static final float IGNITE_PYROMANCER_MOD = 0.9F;
    static final float IGNITE_WIZARD_MOD = 1.05F;

    static final int IGNITE_OVERTIME_DAMAGE = 50;
    static final int IGNITE_OVERTIME_DAMAGE_LVLBONUS = 30;
    static final int IGNITE_OVERTIME_ROUNDS = 2;

    // knight constants
    static final int KNIGHTINITHP = 900;
    static final int KNIGHTHPBONUS = 80;

    static final Character KNIGHTTERRAIN = 'L';
    static final float KNIGHTTERRAINBONUS = 1.15F;

    // --> execute constants
    static final int EXECUTE_BASE_DAMAGE = 200;
    static final int EXECUTE_LVLBONUS_DAMAGE = 30;

    static final float EXECUTE_ROGUE_MOD = 1.15F;
    static final float EXECUTE_KNIGHT_MOD = 1.0F;
    static final float EXECUTE_PYROMANCER_MOD = 1.10F;
    static final float EXECUTE_WIZARD_MOD = 0.8F;

    static final float EXECUTE_INITIAL_HP_LIMIT = 0.2F;
    static final float EXECUTE_BONUS_HP_LIMIT = 0.01F;
    static final float EXECUTE_UPPER_LIMIT = 0.4F;

    // --> slam constants
    static final int SLAM_BASE_DAMAGE = 100;
    static final int SLAM_LVLBONUS_DAMAGE = 40;

    static final float SLAM_ROGUE_MOD = 0.8F;
    static final float SLAM_KNIGHT_MOD = 1.2F;
    static final float SLAM_PYROMANCER_MOD = 0.90F;
    static final float SLAM_WIZARD_MOD = 1.05F;

    // wizard constants
    static final int WIZARDINITHP = 400;
    static final int WIZARDHPBONUS = 30;

    static final Character WIZARDTERRAIN = 'D';
    static final float WIZARDTERRAINBONUS = 1.10F;

    // --> drain constants
    static final float DRAIN_BASE_DAMAGE = 0.20F;
    static final float DRAIN_LVLBONUS_DAMAGE = 0.05F;

    static final float DRAIN_ROGUE_MOD = 0.8F;
    static final float DRAIN_KNIGHT_MOD = 1.2F;
    static final float DRAIN_PYROMANCER_MOD = 0.9F;
    static final float DRAIN_WIZARD_MOD = 1.05F;

    static final float DRAIN_MAX_HP_PERCENT = 0.3F;

    // --> deflect constants
    static final float DEFLECT_BASE_DAMAGE = 0.35F;
    static final float DEFLECT_LVLBONUS_DAMAGE = 0.02F;

    static final float DEFLECT_ROGUE_MOD = 1.2F;
    static final float DEFLECT_KNIGHT_MOD = 1.4F;
    static final float DEFLECT_PYROMANCER_MOD = 1.3F;
    static final float DEFLECT_WIZARD_MOD = 0F;

    static final float DEFLECT_MAX_BONUS_PERCENT = 0.70F;

    // rogue constants
    static final int ROGUEINITHP = 600;
    static final int ROGUEHPBONUS = 40;

    static final Character ROGUETERRAIN = 'W';
    static final float ROGUETERRAINBONUS = 1.15F;

    static final int CRITICAL_HIT_CYCLE = 3;
    static final float CRITICAL_HIT_BONUS = 1.5F;

    // --> backstab constants
    static final int BACKSTAB_BASE_DAMAGE = 200;
    static final int BACKSTAB_LVLBONUS_DAMAGE = 20;

    static final float BACKSTAB_ROGUE_MOD = 1.2F;
    static final float BACKSTAB_KNIGHT_MOD = 0.9F;
    static final float BACKSTAB_PYROMANCER_MOD = 1.25F;
    static final float BACKSTAB_WIZARD_MOD = 1.25F;

    // --> paralysis constants
    static final int PARALYSIS_BASE_DAMAGE = 40;
    static final int PARALYSIS_LVLBONUS_DAMAGE = 10;

    static final float PARALYSIS_ROGUE_MOD = 0.9F;
    static final float PARALYSIS_KNIGHT_MOD = 0.8F;
    static final float PARALYSIS_PYROMANCER_MOD = 1.2F;
    static final float PARALYSIS_WIZARD_MOD = 1.25F;

    static final int PARALYSIS_NORMAL_OVERTIME_ROUNDS = 3;
    static final int PARALYSIS_EXTENDED_OVERTIME_ROUNDS = 6;

    // --> level growth constants
    public static final int LEVEL_GROWTH_BASE = 250;
    public static final int LEVEL_GROWTH_EXPONENT = 50;

    // --> xp growth constants
    public static final int XP_GROWTH_BASE = 200;
    public static final int XP_GROWTH_EXPONENT = 40;
}
