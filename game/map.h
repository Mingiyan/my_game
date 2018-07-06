#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 25;//размер карты высота
const int WIDTH_MAP = 40;//размер карты ширина 


sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0           u                          0",
	"0   s                           u      0",
	"0                                      0",
	"0         00000000                     0",
	"0       s        0          s    0000000",
	"0               00        h            0",
	"0               0                      0",
	"0     h         00                     0",
	"0                0             s       0",
	"0                0          u          0",
	"0 u              0                     0",
	"0                 s    s               0",
	"0  h                                   0",
	"0000000000000                          0",
	"0 h                                    0",
	"0                u           s         0",
	"0        s                             0",
	"0                                      0",
	"0                         00000000000000",
	"0              s                       0",
	"0                          uu          0",
	"0   u     s                      h     0",
	"0 h                                    0",
	"0000000000000000000000000000000000000000",
};

void randomMapGenerate() // рандомно лужицы
{
	int randomElementX = 0; // случайный элемент по горизонтали
	int randomElementY = 0;//случайный элемент по вертикали
	srand(time(0)); // рандом
	int countStone = 1; //количествно луж

	while (countStone > 0)
	{
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);//рандомное по иксу от 1 до ширины-1 чтобы в стене не появилось
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);// тоже по игреку

		if (TileMap[randomElementY][randomElementX] == ' ')
		{
			TileMap[randomElementY][randomElementX] = 's';
			
			countStone--;
		}

		
	}
	int countUrod = 1; //количествно луж

	while (countUrod > 0)
	{
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);//рандомное по иксу от 1 до ширины-1 чтобы в стене не появилось
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);// тоже по игреку

		if (TileMap[randomElementY][randomElementX] == ' ')
		{
			TileMap[randomElementY][randomElementX] = 'u';

			countUrod--;
		}


	}
	int countHealth = 1; //количествно луж

	while (countHealth > 0)
	{
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);//рандомное по иксу от 1 до ширины-1 чтобы в стене не появилось
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);// тоже по игреку

		if (TileMap[randomElementY][randomElementX] == ' ')
		{
			TileMap[randomElementY][randomElementX] = 'h';

			countHealth--;
		}


	}
}