#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h"
#include "view.h"
using namespace sf;



////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
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
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
		if (health <= 0) { life = false; } // если хп меньше или равно 0, то герой помирает
	}


	void interactionWithMap()//ф-ция взаимодействия с картой
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j<(x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy>0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy<0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx>0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				if (TileMap[i][j] == 's') { //если символ равен 's' (уродец)
					health-=10; // если наступить в лужу то будет -10 хп
					TileMap[i][j] = ' ';//лужа так и остается. что то не получается чтобы она оставалась
				}
				if (TileMap[i][j] == 'u') { //если символ равен 's' (уродец)
					health -= 30; // минус 30 хп
					playerScore--;// если взять урода то будет минус очко, 
					TileMap[i][j] = ' ';//убираем урода, чтобы дальше не мешал. 
				}
				if (TileMap[i][j] == 'h') { //если символ равен 's' (уродец)
					playerScore++; // прибавится очко, переменная увеличится
					health += 20; // выпить чай хп увеличится
					TileMap[i][j] = ' ';//убираем чай, типа взяли бонус
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

	Font font; // шрифт
	font.loadFromFile("elmira.ttf"); // передаем нашему шрифту файл шрифта
	Text text("", font, 20); // создаем объект текст, закидываем в объект текст строку, шрифт, размер шрифта
	text.setColor(Color::Red); // покрасили текст в зеленый цвет
	text.setStyle(Text::Bold); // жирный текст

	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);


	Player p("hero.png", 250, 250, 46.0, 79.0);

	float CurrentFrame = 0;
	Clock clock;
	Clock gameTimeClock; // переменная игрового времени
	int gameTime = 0; // инициализировали игровое время
	int createObjectForMapTimer = 0; // таймер для рандома
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

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
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
		
		createObjectForMapTimer += time; // наращиваем таймер
		if (createObjectForMapTimer > 3000)
		{
			randomMapGenerate(); // генерация случайных луж
			createObjectForMapTimer = 0; // обнуляем таймер
		}

		p.update(time);



		window.setView(view);
		window.clear();

		/////////////////////////////Рисуем карту/////////////////////
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

		std::ostringstream playerScoreString, gameTimeString; // объявили переменную
		playerScoreString << p.playerScore; 
		gameTimeString << gameTime;// занесли в нее число очков
		text.setString("vashi ochki: " + playerScoreString.str() + "\nVremya v igre" + gameTimeString.str()); // задает строку тексту
		text.setPosition(view.getCenter().x-220, view.getCenter().y-220); // задаем позицию строки с текстом отступая от центра камеры
		window.draw(text); // рисуем текст

		std::ostringstream playerHealthString;
		playerHealthString << p.health;
		text.setString("zdorovie: " + playerHealthString.str()); // задает строку тексту
		text.setPosition(view.getCenter().x - 220, view.getCenter().y - 240); // задаем позицию строки с текстом отступая от центра камеры
		window.draw(text); // рисуем текст

		window.draw(p.sprite);
		window.display();
	}

	return 0;
}