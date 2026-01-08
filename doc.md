# 设计文档 (Design Document)

本文档旨在描述海战游戏期末大作业的设计架构与核心功能。该项目是一个基于控制台的海战策略游戏，玩家需操控战舰消灭敌方单位，同时收集资源以提升等级。

## 1. 类图 (Class Diagram)

本节展示了系统中的主要类及其关系。

*   **Game (游戏核心类)**
    *   系统的中央控制器，管理游戏状态、主循环 (`run`) 及所有游戏实体。
    *   负责处理用户输入、更新逻辑、渲染画面及管理关卡。

*   **Gui (用户界面类)**
    *   封装 `ncurses` 库，负责界面绘制、颜色管理及字符输出，实现图形与逻辑分离。

*   **GameObject (游戏对象基类)**
    *   所有可视实体的基类，定义了坐标 `(row, col)`、颜色 `color` 及状态 `active`。
    *   提供虚函数 `draw()` 供派生类实现绘制。
    *   **派生类：**
        *   **Obstacle (障碍物)**: 代表地图中的阻挡物（如岛屿），阻挡移动及炮弹。
        *   **Item (道具)**: 代表增益物品，如武器包和医疗包。
        *   **Weapon (武器)**: 代表攻击性投射物，含炮弹、鱼雷及导弹。
        *   **Bomber (轰炸机)**: 空中单位，负责投掷炸弹。
        *   **Ship (舰船基类)**: 战舰基类，含生命值及移动逻辑。
            *   **BattleShip (玩家战舰)**: 玩家角色，具备升级及多武器发射能力。
            *   **EnemyShip (敌舰基类)**: 敌方单位基类，定义通用 AI。
                *   **GunBoat (炮艇)**: 轻型敌舰，灵活，普通炮击。
                *   **Destroyer (驱逐舰)**: 中型敌舰，能发射鱼雷。
                *   **Cruiser (巡洋舰)**: 重型敌舰，防御高，全方位火力。

---

## 2. 核心用例 (Use Cases)

本节描述游戏中的关键交互流程。

### 用例 1: 玩家移动 (Player Movement)

*   **Name**: 玩家控制战舰移动
*   **Brief Description**: 玩家通过键盘控制战舰移动，系统确保不越界、不穿墙。
*   **Actors**: 玩家 (Player)
*   **Preconditions**: 游戏运行中，玩家战舰存活。
*   **Basic Flow**:
    1.  玩家按方向键。
    2.  系统计算目标坐标。
    3.  系统检测是否越界。
    4.  系统检测是否有障碍物。
    5.  若合法，系统更新坐标并刷新显示。
*   **Alternate Flows**: 无。
*   **Exception Flows**:
    *   若遇阻挡，战舰保持原位。
*   **Post Conditions**: 战舰位置更新或保持不变。

### 用例 2: 玩家开火 (Player Attack)

*   **Name**: 玩家发射武器
*   **Brief Description**: 玩家选择武器攻击，系统处理消耗及生成。
*   **Actors**: 玩家 (Player)
*   **Preconditions**: 玩家有对应弹药。
*   **Basic Flow**:
    1.  玩家输入指令：空格（主炮）、`A/D`（齐射）或 `U`（导弹）。
    2.  系统校验弹药。
    3.  若充足，扣除弹药。
    4.  系统生成 `Weapon` 对象。
        *   **侧舷齐射**：生成三发炮弹。
        *   **导弹**：生成追踪导弹。
*   **Alternate Flows**:
    *   若弹药不足，不执行动作。
*   **Exception Flows**: 无。
*   **Post Conditions**: 场景新增武器，弹药减少。

### 用例 3: 敌人行动 (Enemy Action)

*   **Name**: 敌方 AI 行动
*   **Brief Description**: 系统控制敌方移动及攻击。
*   **Actors**: 系统 (System) / 敌舰 (EnemyShip)
*   **Preconditions**: 敌舰存活。
*   **Basic Flow**:
    1.  系统轮询敌舰，概率决定是否追踪。
    2.  若移动，敌舰尝试靠近玩家。
    3.  检测玩家是否在攻击范围内。
    4.  若满足条件且冷却结束，敌舰开火。
    5.  不同敌舰执行不同攻击模式。
*   **Alternate Flows**: 无。
*   **Exception Flows**:
    *   若无法命中，敌舰待机。
*   **Post Conditions**: 敌舰坐标更新或新增武器。

### 用例 4: 升级 (Level Up)

*   **Name**: 关卡晋升
*   **Brief Description**: 玩家积累资源后提升难度。
*   **Actors**: 系统 (System)
*   **Preconditions**: 金币达 1000。
*   **Basic Flow**:
    1.  系统检测金币。
    2.  若达标，扣除金币。
    3.  等级 `level` 加 1。
    4.  系统缩短敌人生成间隔。
    5.  显示 "LEVEL UP!"。
*   **Alternate Flows**: 无。
*   **Exception Flows**: 无。
*   **Post Conditions**: 进入下一级，生成加快。

### 用例 5: 游戏结束 (Game Over)

*   **Name**: 判定玩家失败
*   **Brief Description**: 玩家生命值耗尽，游戏终止。
*   **Actors**: 系统 (System)
*   **Preconditions**: 玩家受到伤害。
*   **Basic Flow**:
    1.  玩家战舰被敌方武器击中。
    2.  系统扣除战舰生命值。
    3.  系统检测剩余生命值。
    4.  若生命值 <= 0，系统判定玩家死亡。
    5.  界面显示 "GAME OVER"。
    6.  等待按键退出程序。
*   **Alternate Flows**: 无。
*   **Exception Flows**: 无。
*   **Post Conditions**: 游戏程序结束。

### 用例 6: 地图加载 (Map Loading)

*   **Name**: 初始化游戏地图
*   **Brief Description**: 游戏启动时加载地形数据。
*   **Actors**: 系统 (System)
*   **Preconditions**: 游戏启动。
*   **Basic Flow**:
    1.  系统尝试读取 `map.txt` 文件。
    2.  若文件存在，解析字符（`#` 代表障碍物）。
    3.  根据解析结果生成 `Obstacle` 对象。
*   **Alternate Flows**:
    *   若文件不存在，系统执行随机生成算法。
    *   在地图区域内随机生成群岛状障碍物。
*   **Exception Flows**: 无。
*   **Post Conditions**: 游戏地图包含障碍物数据。
