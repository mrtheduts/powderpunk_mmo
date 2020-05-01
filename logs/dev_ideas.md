# Dev Ideas record

## 2020-05-01
### Implementation Ideas
- Changed imgui to Qt5 (imgui was not what I was looking for);

## 2020-04-27
### Design Ideas
So, I'm thinking of a text-based MMORPG, like MUDs, but with a living world.
 - NPCs could age and have children;
 - Players could buy plot lands and build or buy houses;
 - Extensive crafting;
 - Players could live as they like;
 
### Implementation Ideas
 - NCurses for multiple windows in a terminal, to start with;
 - Or use as a application with imgui (I think that's better);
 - One thread for each window;
 - Each thread has its own connection;
 - Or the thread has local information - like inventory;
 - A sole connection for the map;
