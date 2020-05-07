package hero;

public interface VisitorHero {
     int giveDamage(Pyromancer pyromancer);
     int giveDamage(Knight knight);
     int giveDamage(Rogue rogue);
     int giveDamage(Wizard wizard);
}
