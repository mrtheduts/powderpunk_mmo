# Dev Ideas record

## 2020-08-21
### Design Ideas
- Standalone client cancelled. Only telnet remains for text (check integration with telegram bots to use it);

### Implementation Ideas
- Godot for developing graphical client;
- Lock for every entity in the world - each player is a thread;
- NPCs of an area controlled in rounds - maybe set a roundrobin scheme of priority;
- If every NPC is thread, SO scheduling scheme will be used? No need to implement it, then;

## 2020-06-13
### Design Ideas
- Crafting as fun as FFXIV;
- Item quality;
- Gambits - increase size/diversity with experience/witnessed situations;
- Companions (with gambits);

## 2020-05-04
### Design Ideas
- Try to assemble telnet, client and graphical client in one server application.

## 2020-05-03
### Design Ideas
- Abstract HP to just percentage - no absolute values;

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
