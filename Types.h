#ifndef _TYPES_H_
#define _TYPES_H_

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
enum EnemyType { GUNBOAT, DESTROYER, CRUISER }; // 炮艇, 驱逐舰, 巡洋舰
enum WeaponType { SHELL, TORPEDO, MISSILE }; // 炮弹, 鱼雷, 导弹
enum ItemType { WEAPON_PACK, MEDICAL_PACK }; // 武器包, 医疗包

// Color constants matching Gui.C init_pair
const int COL_ENEMY = 1;
const int COL_PLAYER = 2;
const int COL_ITEM = 3;
const int COL_OBSTACLE = 4;
const int COL_BOMBER = 5; // Gunboat cyan
const int COL_MISSILE = 6;
const int COL_DEFAULT = 7;
const int COL_DESTROYER_RED = 8; // Renamed for clarity

#endif
