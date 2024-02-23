#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <string>
#include <Windows.h>
#include <SFML/Audio.hpp>
#include <sstream>

//Main vars
float windowWidth = 1280;
float windowHeight = 720;


int random(int a, int b)
{
	srand(time(NULL));
	if (a > 0) return a + rand() % (b - a);
	else return a + rand() % (abs(a) + b);
}



int main() {
	//Global Vars
	int totalHits = -2;
	int leftWins = 0;
	int rightWins = 0;
	

	//Renders
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "PING-PONG!");
	window.setFramerateLimit(60);
	sf::SoundBuffer leftBuffer;
	sf::SoundBuffer rightBuffer;
	sf::SoundBuffer winBuffer;
	sf::SoundBuffer winBuffer1;


	if (!leftBuffer.loadFromFile("D:\\Python\\Code\\C++\\Source\\Audio\\hit.ogg")) {
		std::cout << "ERROR" << std::endl;
	}
	if (!rightBuffer.loadFromFile("D:\\Python\\Code\\C++\\Source\\Audio\\hit1.ogg")) {
		std::cout << "ERROR" << std::endl;
	}
	if (!winBuffer.loadFromFile("D:\\Python\\Code\\C++\\Source\\Audio\\win.ogg")) {
		std::cout << "ERROR" << std::endl;
	}
	if (!winBuffer1.loadFromFile("D:\\Python\\Code\\C++\\Source\\Audio\\win1.ogg")) {
		std::cout << "ERROR" << std::endl;
	}


	//Sounds
	sf::Sound win1;
	sf::Sound win;
	sf::Sound pongSound;
	sf::Sound pingSound;
	pongSound.setBuffer(leftBuffer);
	pingSound.setBuffer(rightBuffer);
	win.setBuffer(winBuffer);
	win1.setBuffer(winBuffer1);

	//Fonts and Texts
	sf::Font font;
	font.loadFromFile("D:\\Python\\Code\\C++\\Source\\Fonts\\minecraft.ttf");

	//Hits
	sf::Text main_text;
	main_text.setFont(font);
	main_text.setPosition(125, 40);
	main_text.setScale(0.8, 0.8);

	//Left Wins
	sf::Text leftWins_txt;
	leftWins_txt.setFont(font);
	leftWins_txt.setPosition(300, 150);
	leftWins_txt.setScale(2.f, 2.f);
	leftWins_txt.setFillColor(sf::Color::Red);

	//Right Wins
	sf::Text rightWins_txt;
	rightWins_txt.setFont(font);
	rightWins_txt.setPosition(920, 150);
	rightWins_txt.setScale(2.f, 2.f);
	rightWins_txt.setFillColor(sf::Color::Blue);

	

	/* _____________________SETTINGS_____________________*/



	//Player Vars
	float playerVel = 7.5;
	float playerWidth = 20.f;
	float playerHeight = 200.f;
	float playerVelIncrease = 0.2;


	//Offset
	float offset_collision_static_left = 14.f;
	float offset_collision_static_right = 14;
	float offset_collision_change = 1.2;

	
	//Ball Vars
	float ballSize = 25;
	float ballVel_original = 6;
	float xBallVel = ballVel_original;
	float yBallVel = ballVel_original;
	float ballAcceleration = 0.25;


	//Left Player
	sf::RectangleShape leftPlayer;
	sf::Vector2f leftPlayerPos(75, 250);
	leftPlayer.setSize(sf::Vector2f(playerWidth, playerHeight));
	leftPlayer.setFillColor(sf::Color::Red);



	//Right Player
	sf::RectangleShape rightPlayer;
	sf::Vector2f rightPlayerPos(1205, 250);
	rightPlayer.setSize(sf::Vector2f(playerWidth, playerHeight));
	rightPlayer.setFillColor(sf::Color::Blue);



	//Line
	sf::RectangleShape line;
	sf::Vector2f linePos(627, 0);
	line.setSize(sf::Vector2f(2, windowHeight));
	line.setFillColor(sf::Color::Yellow);
	line.setPosition(linePos);


	//Ball
	sf::RectangleShape ball;
	sf::Vector2f ballPos(615, 360);
	ball.setSize(sf::Vector2f(ballSize, ballSize));
	ball.setFillColor(sf::Color::White);


	/*_______________________________________________*/


	
	//Main Loop
	while (window.isOpen()) {
		sf::FloatRect leftPlayer_boundingBox = leftPlayer.getGlobalBounds();
		sf::FloatRect rightPlayer_boundingBox = rightPlayer.getGlobalBounds();
		sf::FloatRect ball_boundingBox = ball.getGlobalBounds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}


		//Left Player Movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && leftPlayerPos.y > 0)  {
			leftPlayerPos.y -= playerVel;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && leftPlayerPos.y < windowHeight-playerHeight) {
			leftPlayerPos.y += playerVel;
		}

		//Right Player Movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rightPlayerPos.y > 0) {
			rightPlayerPos.y -= playerVel;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && rightPlayerPos.y < windowHeight - playerHeight) {
			rightPlayerPos.y += playerVel;
		}
		


		//Ball Movement Simple
		ballPos.x += xBallVel;
		ballPos.y += yBallVel;
		
		//Ball Collision Windows
		if (ballPos.x < 0 || ballPos.x > windowWidth - ballSize) xBallVel *= -1;
		if (ballPos.y < 0 || ballPos.y > windowHeight - ballSize) yBallVel *= -1;


		//Ball Collision Player Left
		if (leftPlayer_boundingBox.intersects(ball_boundingBox)) {
			pongSound.play();
			ballPos.x += offset_collision_static_left;
			xBallVel *= -1;
			playerVel += playerVelIncrease;
			totalHits += 1;
			

			//Left Acceleration
			if (xBallVel <= 0) {
				xBallVel -= ballAcceleration;
				offset_collision_static_left += offset_collision_change;
			}
			else if (xBallVel >= 0) {
				xBallVel += ballAcceleration;
				offset_collision_static_left += offset_collision_change;
			}
		}


		//Ball Collision Player Right
		if (rightPlayer_boundingBox.intersects(ball_boundingBox)) {
			pingSound.play();
			ballPos.x -= offset_collision_static_right;
			xBallVel *= -1;
			totalHits += 1;
			

			//Right Acceleration
			if (xBallVel <= 0) {
				xBallVel -= ballAcceleration;
				offset_collision_static_right += offset_collision_change;
			}
			else if (xBallVel >= 0) {
				xBallVel += ballAcceleration;
				offset_collision_static_right += offset_collision_change;
			}
		}


		//Check if win:

		//Right
		if (ballPos.x <= 0) {
			win1.play();
			printf("Right Wins!\n");
			rightWins += 1;
			totalHits = 0;
			sf::Vector2f newLeftPlayerPos(75, 250);
			sf::Vector2f newRightPlayerPos(1205, 250);
			sf::Vector2f newBallPos(615, 360);
			leftPlayerPos = newLeftPlayerPos;
			rightPlayerPos = newRightPlayerPos;
			ballPos = newBallPos;
			xBallVel = ballVel_original;
			yBallVel = ballVel_original;
			Sleep(1000);
		}

		//Left
		if (ballPos.x >= windowWidth-ballSize) {
			win.play();
			printf("Left Wins!\n");
			leftWins += 1;
			totalHits = 0;
			sf::Vector2f newLeftPlayerPos(75, 250);
			sf::Vector2f newRightPlayerPos(1205, 250);
			sf::Vector2f newBallPos(615, 360);
			leftPlayerPos = newLeftPlayerPos;
			rightPlayerPos = newRightPlayerPos;
			ballPos = newBallPos;
			xBallVel = ballVel_original;
			yBallVel = ballVel_original;
			Sleep(1000);
		}

		
/*
		NOTHING SHOULD BE BELOW!
*/

		ball.setPosition(ballPos);
		rightPlayer.setPosition(rightPlayerPos);
		leftPlayer.setPosition(leftPlayerPos);
		window.clear();

		//Total Hits Draw
		std::ostringstream totalHitsString;
		totalHitsString << totalHits;
		main_text.setString("Total Hits: " + totalHitsString.str());

		//Left Wins Draw
		std::ostringstream winsLeftString;
		winsLeftString << leftWins;
		leftWins_txt.setString(winsLeftString.str());

		//Right Wins Draw
		std::ostringstream winsRightString;
		winsRightString << rightWins;
		rightWins_txt.setString(winsRightString.str());


		//Drawings
		window.draw(leftWins_txt);
		window.draw(rightWins_txt);
		window.draw(main_text);
		window.draw(ball);
		window.draw(line);
		window.draw(leftPlayer);
		window.draw(rightPlayer);
		window.display();
	}




	return 0;
}
