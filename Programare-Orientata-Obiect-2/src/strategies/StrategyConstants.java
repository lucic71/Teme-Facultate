package strategies;

public class StrategyConstants {
    // --> Knight constants
    public static float KNIGHT_DECREASE_HP  = 0.80f;
    public static float KNIGHT_INCREASE_MOD = 0.50f;

    public static float KNIGHT_INCREASE_HP  = 1.25f;
    public static float KNIGHT_DECREASE_MOD = -0.20f;

    public static float KNIGHT_LOWER_LIMIT = 1/3f;
    public static float KNIGHT_UPPER_LIMIT = 1/2f;

    // --> Pyro constants
    public static float PYRO_DECREASE_HP  = 0.75f;
    public static float PYRO_INCREASE_MOD = 0.70f;

    public static float PYRO_INCREASE_HP  = 1.33f;
    public static float PYRO_DECREASE_MOD = -0.30f;

    public static float PYRO_LOWER_LIMIT = 1/4f;
    public static float PYRO_UPPER_LIMIT = 1/3f;

    // --> Rogue constants
    public static float ROGUE_DECREASE_HP  = (1-1/7f);
    public static float ROGUE_INCREASE_MOD = 0.40f;

    public static float ROGUE_INCREASE_HP  = 1.50f;
    public static float ROGUE_DECREASE_MOD = -0.10f;

    public static float ROGUE_LOWER_LIMIT = 1/7f;
    public static float ROGUE_UPPER_LIMIT = 1/5f;

    // --> Wizard constants
    public static float WIZARD_DECREASE_HP  = (1-1/10f);
    public static float WIZARD_INCREASE_MOD = 0.60f;

    public static float WIZARD_INCREASE_HP  = 1.20f;
    public static float WIZARD_DECREASE_MOD = -0.20f;

    public static float WIZARD_LOWER_LIMIT = 1/4f;
    public static float WIZARD_UPPER_LIMIT = 1/2f;
}
