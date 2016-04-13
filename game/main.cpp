#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h"
#include "view.h"
using namespace sf;



////////////////////////////////////////////////////����� ������////////////////////////
class Player {
private: float x, y;
public:
	float w, h, dx, dy, speed;
	int dir, playerScore, health;
	bool life;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, float X, float Y, float W, float H)
	{
		dir = 0; playerScore = 0;
		dx = 0; dy = 0; speed = 0; health = 100;
		File = F; life = true;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}
	void update(float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		case 2: dx = 0; dy = speed; break;
		case 3: dx = 0; dy = -speed; break;
		}

		x += dx*time;
		y += dy*time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
		if (health <= 0) { life = false; } // ���� �� ������ ��� ����� 0, �� ����� ��������
	}


	void interactionWithMap()//�-��� �������������� � ������
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; j<(x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
			{
				if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
				{
					if (dy>0)//���� �� ��� ����,
					{
						y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
					}
					if (dy<0)
					{
						y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
					}
					if (dx>0)
					{
						x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//���������� ���� �����
					}
				}

				if (TileMap[i][j] == 's') { //���� ������ ����� 's' (������)
					health-=10; // ���� ��������� � ���� �� ����� -10 ��
					TileMap[i][j] = ' ';//���� ��� � ��������. ��� �� �� ���������� ����� ��� ����������
				}
				if (TileMap[i][j] == 'u') { //���� ������ ����� 's' (������)
					health -= 30; // ����� 30 ��
					playerScore--;// ���� ����� ����� �� ����� ����� ����, 
					TileMap[i][j] = ' ';//������� �����, ����� ������ �� �����. 
				}
				if (TileMap[i][j] == 'h') { //���� ������ ����� 's' (������)
					playerScore++; // ���������� ����, ���������� ����������
					health += 20; // ������ ��� �� ����������
					TileMap[i][j] = ' ';//������� ���, ���� ����� �����
				}
			}
	}


	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}

};



int main()
{
	setlocale(LC_ALL, "Russian");
	RenderWindow window(sf::VideoMode(640, 480), "game");
	view.reset(sf::FloatRect(0, 0, 640, 480));

	Font font; // �����
	font.loadFromFile("elmira.ttf"); // �������� ������ ������ ���� ������
	Text text("", font, 20); // ������� ������ �����, ���������� � ������ ����� ������, �����, ������ ������
	text.setColor(Color::Red); // ��������� ����� � ������� ����
	text.setStyle(Text::Bold); // ������ �����

	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);


	Player p("hero.png", 250, 250, 46.0, 79.0);

	float CurrentFrame = 0;
	Clock clock;
	Clock gameTimeClock; // ���������� �������� �������
	int gameTime = 0; // ���������������� ������� �����
	int createObjectForMapTimer = 0; // ������ ��� �������
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); 
		else
		{
			view.rotate(0.01);
		}
		clock.restart();
		time = time / 800;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float coordinatePlayerX, coordinatePlayerY = 0;
		coordinatePlayerX = p.getplayercoordinateX();
		coordinatePlayerY = p.getplayercoordinateY();

		///////////////////////////////////////////���������� ���������� � ���������////////////////////////////////////////////////////////////////////////
		if (p.life) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				p.dir = 1; p.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 109, 40, 74));
			}

			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				p.dir = 0; p.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 209, 40, 74));
			}

			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				p.dir = 3; p.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 301, 42, 80));
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				p.dir = 2; p.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 5, 42, 79));
			}
		}
		getplayercoordinateforview(coordinatePlayerX, coordinatePlayerY);
		
		createObjectForMapTimer += time; // ���������� ������
		if (createObjectForMapTimer > 3000)
		{
			randomMapGenerate(); // ��������� ��������� ���
			createObjectForMapTimer = 0; // �������� ������
		}

		p.update(time);



		window.setView(view);
		window.clear();

		/////////////////////////////������ �����/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if (TileMap[i][j] == 'u') s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}

		std::ostringstream playerScoreString, gameTimeString; // �������� ����������
		playerScoreString << p.playerScore; 
		gameTimeString << gameTime;// ������� � ��� ����� �����
		text.setString("vashi ochki: " + playerScoreString.str() + "\nVremya v igre" + gameTimeString.str()); // ������ ������ ������
		text.setPosition(view.getCenter().x-220, view.getCenter().y-220); // ������ ������� ������ � ������� �������� �� ������ ������
		window.draw(text); // ������ �����

		std::ostringstream playerHealthString;
		playerHealthString << p.health;
		text.setString("zdorovie: " + playerHealthString.str()); // ������ ������ ������
		text.setPosition(view.getCenter().x - 220, view.getCenter().y - 240); // ������ ������� ������ � ������� �������� �� ������ ������
		window.draw(text); // ������ �����

		window.draw(p.sprite);
		window.display();
	}

	return 0;
}