#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <bangtal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

SceneID scene1;
ObjectID gameboard[9], start, OGboard[9];
int blank = 8;
bool game = false;

TimerID timer;
int shuffCount;

int indexToX(int i) {
	return 338 + 200 * (i % 3);
}

int indexToY(int i) {
	return 453 - 200 * (i / 3);
}

int gameindex(ObjectID object) {
	for (int i = 0; i < 9; i++) {
		if (gameboard[i] == object) return i;
	}
	return -1; 
}

void gamemove(int i) {
	ObjectID object = gameboard[i];
	gameboard[i] = gameboard[blank];
	locateObject(gameboard[i], scene1, indexToX(i), indexToY(i));
	gameboard[blank] = object;
	locateObject(gameboard[blank], scene1, indexToX(blank), indexToY(blank));

	blank = i;

}

void gameprint() {
	for (int i = 0; i < 9; i++) {
		printf("%d", gameboard[i]);
	}
	printf("\n");

	time_t start, end;
	double result;
	start = time(NULL);
	end = time(NULL);
	result = (double)(end - start);
	printf("%f", result);
	
	

}

bool possiblemove(int i) {
	if (i % 3 > 0 && blank == i - 1) return true;
	if (i % 3 < 2 && blank == i + 1) return true;
	if (i / 3 > 0 && blank == i - 3) return true;
	if (i / 3 < 2 && blank == i + 3) return true;

	return false;
	
}

int ARBmove() {
	int i = -1;
	
	while (i == -1) {
		switch (rand() % 4) {
		case 0: if (blank % 3 > 0) i = blank - 1;
			break;
        case 1: if (blank % 3 < 2) i = blank + 1;
			break;
		case 2: if (blank / 3 > 0) i = blank - 3;
		    break;
		case 3: if (blank / 3 < 2) i = blank + 3;
			break;
		}
	}
	return i;
}

bool gameend() {
	for (int i = 0; i < 9; i++) {
		if (gameboard[i] != OGboard[i]) return false;
	}
	return true;

	}




void gamestart() {
	blank = 8;
	hideObject(gameboard[blank]);
	shuffCount = 25;
	setTimer(timer, 0.1f);
	startTimer(timer);

	
	



}





void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (game) {
        int i = gameindex(object);

		if (possiblemove(i)) {
			gamemove(i);

		    if (gameend()) {
			    game = false;
			    showObject(start);
				showObject(gameboard[blank]);
				showMessage("ÆÛÁñ ¿Ï¼º!!");
		}
	 

		gameprint();
	}
}
	else {
		if (object == start) {
			gamestart();
		}
	}
}

void timerCallback(TimerID timer)  {

	gamemove(ARBmove());
	shuffCount--;
	if (shuffCount > 0) {
		setTimer(timer, 0.1f);
		startTimer(timer);
	} 
	else { 
		game = true;
		hideObject(start);

	}

}


ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

  
 SceneID gamego() {
	scene1 = createScene("ÇÇÄ«ÃòÆÛÁñ", "Images/¿øº».png");

	char path[50];
	for (int i = 0; i < 9; i++) {
		sprintf(path, "Images/%d.jpg", i + 1);
		gameboard[i] = createObject(path, scene1, indexToX(i), indexToY(i));
		OGboard[i] = gameboard[i];
	}
	start = createObject("Images/start.png", scene1, 590, 100);

	timer = createTimer(0.1f);

	return scene1;
}






int main() {
	




	srand(time(NULL));
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	gamego();
    startGame(scene1);

	
}