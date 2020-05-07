package angel;

import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public interface VisitorAngel {
    void applyAngelAbility(Pyromancer pyromancer);
    void applyAngelAbility(Knight knight);
    void applyAngelAbility(Rogue rogue);
    void applyAngelAbility(Wizard wizard);
}
