// Include important C++ libs here
#include "stdafx.h"
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Branch sides
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	// Video Mode Object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!", Style::Fullscreen);

	// Create Texture to hold graphic in GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach tecture to sprite
	spriteBackground.setTexture(textureBackground);

	// Set sprite background position
	spriteBackground.setPosition(0, 0);

	// Make a tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Background trees
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree1;
	spriteTree1.setTexture(textureTree2);
	spriteTree1.setPosition(405, -200);
	spriteTree1.setScale(0.5, 1.0);
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	spriteTree2.setPosition(-30, -100);
	spriteTree2.setScale(0.75, 1.0);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	spriteTree3.setPosition(1300, -100);
	spriteTree3.setScale(0.75, 1.0);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	spriteTree4.setPosition(1820, -200);
	spriteTree4.setScale(0.5, 1.0);

	// Make the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(-100, 800);

	// Is the bee moving?
	bool beeActive = false;
	short beeDirection;

	// How fast the bee is flying
	float beeSpeed = 0.0f;

	// Make the clouds
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	// Position clouds
	spriteCloud1.setPosition(-200, 0);
	spriteCloud2.setPosition(-200, 250);
	spriteCloud3.setPosition(-200, 500);

	// Clouds on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	// How fast the clouds are moving
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// For fps control
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track if game is paused
	bool paused = true;

	// Draw some text
	int score = 0;

	Text messageText;
	Text scoreText;

	// Choose font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set font to message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Choose Color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// Set actual message
	messageText.setString("Press enter to start!");
	scoreText.setString("Score = 0");

	//Position Text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	// Make 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// set the texture for sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	// Make player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(725, 720);
	spritePlayer.setScale(-1.0, 1.0);

	// Player starts on left
	side playerSide = side::LEFT;

	// Make gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(-600, 860);

	// Make axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Make flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 780);

	// Some other usefull log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	// Prepare the sound
	// Cutting sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// Death sound
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Out of time sound
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		/*
		############################################
					Handle Player Input
		############################################
		*/

		// Exit if Esc pressed
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			score = 0;
			timeRemaining = 5;

			// Make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			// Make gravestone hidden
			spriteRIP.setPosition(675, 2000);

			// Move player into position
			spritePlayer.setPosition(725, 720);
			spritePlayer.setScale(-1.0, 1.0);

			acceptInput = true;

		}

		// Wrap player controls to avoid chopping too fast
		if (acceptInput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
				spritePlayer.setScale(1.0, 1.0);

				updateBranches(score);

				spriteLog.setPosition(810, 780);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Play chop sound
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(725, 720);
				spritePlayer.setScale(-1.0, 1.0);

				updateBranches(score);

				spriteLog.setPosition(810, 780);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Play chop sound
				chop.play();
			}
		}

		/*
		############################################
					Update The Scene
		############################################
		*/
		if (!paused) {
			Time dt = clock.restart();

			// Subtract the amount of time remaining
			timeRemaining -= dt.asSeconds();

			// Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				paused = true;

				messageText.setString("Out of time!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play out of time sound
				outOfTime.play();
			}

			// Setup bee
			if (!beeActive) {
				// How fast is the bee?
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;
				srand((int)time(0));
				beeDirection = (rand() % 2) + 1;
				char msgbuf[10];
				sprintf_s(msgbuf, "%hi", beeDirection);
				OutputDebugStringA(msgbuf);

				// How 420 is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				if (beeDirection == 1) {
					spriteBee.setPosition(2000, height);
					spriteBee.setScale(1.f, 1.f);
				}
				else {
					spriteBee.setPosition(-200, height);
					spriteBee.setScale(-1.f, 1.f);
				}
				beeActive = true;

			}
			else {
				if (beeDirection == 1) {
					spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				}
				else {
					spriteBee.setPosition(spriteBee.getPosition().x + (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				}
				if (spriteBee.getPosition().x < -100 && beeDirection == 1) {
					beeActive = false;
				}
				else if (spriteBee.getPosition().x > 2000 && beeDirection == 2) {
					beeActive = false;
				}
			}

			// Manage clouds
			// Cloud 1
			if (!cloud1Active) {
				//How fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How 420 is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 1920) {
					cloud1Active = false;
				}
			}
			if (!cloud2Active) {
				//How fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How 420 is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1920) {
					cloud2Active = false;
				}
			}
			if (!cloud3Active) {
				//How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// How 420 is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1920) {
					cloud3Active = false;
				}
			}

			// Score update
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// Update branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else {
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
					logActive = false;
					spriteLog.setPosition(810, 780);
				}
			}

			// Player death
			if (branchPositions[5] == playerSide) {
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				if (playerSide == side::LEFT) {
					spriteRIP.setPosition(585, 760);
				}
				else {
					spriteRIP.setPosition(1200, 760);
				}

				// Hide the player
				spritePlayer.setPosition(2300, 660);

				// Change the text of the message
				messageText.setString("SQUISHED!!");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play death sound
				death.play();
			}
		}
		/*
		############################################
					Draw The Scene
		############################################
		*/

		// Clear last frame
		window.clear();

		// Draw game scene here
		window.draw(spriteBackground);

		// Draw clouds
		window.draw(spriteCloud1);
		//window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// Draw backdrop trees
		window.draw(spriteTree1);
		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);

		// Draw branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		// Draw tree
		window.draw(spriteTree);

		// Draw Player
		window.draw(spritePlayer);

		// Draw axe
		window.draw(spriteAxe);

		// Draw flying log
		window.draw(spriteLog);

		// Draw gravestone
		window.draw(spriteRIP);

		// Draw bee
		window.draw(spriteBee);

		// Draw score
		window.draw(scoreText);

		// Draw timebar
		window.draw(timeBar);

		if (paused) {
			// Draw Message
			window.draw(messageText);
		}

		// Show everything we drew
		window.display();
	}

    return 0;
}

// Function for branches
void updateBranches(int seed) {

	// Move branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn new branch
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}

}

