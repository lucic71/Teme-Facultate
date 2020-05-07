package engine;

import hero.AbstractHero;

class MovementEngine {
    final void move(final AbstractHero hero, final char direction) {
        switch (direction) {
            case 'L' :
                hero.setPosY(hero.getPosY() - 1);
                break;

            case 'R' :
                hero.setPosY(hero.getPosY() + 1);
                break;

            case 'D' :
                hero.setPosX(hero.getPosX() + 1);
                break;

            case 'U' :
                hero.setPosX(hero.getPosX() - 1);
                break;

            default:
                break;
        }
    }
}
