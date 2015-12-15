#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
using namespace sf;

////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player {// класс Игрока
private: float x, y = 0;
public:
	float  w, h, dx, dy, speed = 0,health;//координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
	bool life;
	int dir = 0,playerScore; //направление (direction) движения игрока
	String File; //файл с расширением
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Texture textureHead;
	Sprite sprite;//сфмл спрайт
	Sprite spriteHead;

	Player(String F, float X, float Y, float W, float H){//Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		playerScore=0;//направление (direction) движения игрока
		life = true;
		health = 100;
		File = F;//имя файла+расширение
		w = W; h = H;//высота и ширина
		image.loadFromFile(File);//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта. В нашем случае "hero.png" и получится запись идентичная image.loadFromFile("images/hero/png");
		image.createMaskFromColor(Color(41, 33, 59));//убираем ненужный темно-синий цвет, эта тень мне показалась не красивой.
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);
		textureHead.loadFromImage(image);//закидываем наше изображение в текстуру
		spriteHead.setTexture(textureHead);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 75, w, h));//Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов
		spriteHead.setTextureRect(IntRect(0, 19, w, h));
	}

	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{

		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0; break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0; break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed; break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed; break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		x += dx*time;//то движение из прошлого урока. наше ускорение на время получаем смещение координат и как следствие движение
		y += dy*time;//аналогично по игреку

		speed = 0;//зануляем скорость, чтобы персонаж остановился.

		sprite.setPosition(x, y);//выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		spriteHead.setPosition(x, y);
		interactionWithMap();
		if (health <= 0){ life = false; speed = 0; }
	}
		float getplayercoordinateX(){	//этим методом будем забирать координату Х
			return x;
		}
		float getplayercoordinateY(){	//этим методом будем забирать координату Y
			return y;
		}

		void interactionWithMap()//ф-ция взаимодействия с картой
		{

			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
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

				if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
					//x = 300; y = 300;//какое то действие... например телепортация героя
					playerScore++;
					TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
				}
				if (TileMap[i][j] == 'f') {
					health -= 40;//если взяли ядовитейший в мире цветок,то переменная health=health-40;
					TileMap[i][j] = ' ';//убрали цветок
				}

				if (TileMap[i][j] == 'h') {
					health += 20;//если взяли сердечко,то переменная health=health+20;
					TileMap[i][j] = ' ';//убрали сердечко
				}
			}
		}


};




int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Isaac");
	view.reset(sf::FloatRect(0, 0, 640, 480));
	float CurrentFrame = 0;//хранит текущий кадр
	Clock clock;
	Font font;//шрифт
	font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setColor(Color::Yellow);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	Text textwin("", font, 50);
	textwin.setColor(Color::Red);
	textwin.setStyle(sf::Text::Bold | sf::Text::Underlined);

	Image map_image;//объект изображения для карты
	map_image.loadFromFile("map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

		Player p("isaac.png", 320, 240, 32.0, 15.0);
		Player h("isaac.png", 320, 218, 34.0, 28.0);
		Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
		int gameTime = 0;
	while (window.isOpen())
	{


		float time = clock.getElapsedTime().asMicroseconds();
		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 500;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (p.life){
			if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 2; p.speed = 0.1;//направление вверх, см выше
				h.dir = 2; h.speed = 0.1;
				CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 10) CurrentFrame -= 10; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				p.sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 75, 32, 15));//проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
				h.spriteHead.setTextureRect(IntRect(0, 19, 34, 28)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96


			}
			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 3; p.speed = 0.1;//направление вверх, см выше
				h.dir = 3; h.speed = 0.1;
				CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 10) CurrentFrame -= 10; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				p.sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 75, 32, 15)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
				h.spriteHead.setTextureRect(IntRect(0, 19, 34, 28));
			}
			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 0; p.speed = 0.1;
				h.dir = 0; h.speed = 0.1;//направление вверх, см выше
				CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 10) CurrentFrame -= 10; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				p.sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 118, 32, 15)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
				h.spriteHead.setTextureRect(IntRect(0, 19, 34, 28));

			}
			if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 1; p.speed = 0.1;//направление вверх, см выше
				h.dir = 1; h.speed = 0.1;
				CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 10) CurrentFrame -= 10; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				p.sprite.setTextureRect(IntRect(32 * int(CurrentFrame) + 32, 118, -32, 15)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
				h.spriteHead.setTextureRect(IntRect(32, 19, -34, 28));
			}
		}
		p.update(time);
		h.update(time);
		viewmap(time);

		window.setView(view);
		window.clear(Color::White);
		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
			if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
			if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
			if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//добавили цветок
			if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));

			s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

			window.draw(s_map);//рисуем квадратики на экран
		}
		if (p.playerScore == 3)
		{

			textwin.setString("ВЫ ВЫЙГРАЛИ");
			textwin.setPosition(view.getCenter().x - 150, view.getCenter().y);
			window.draw(textwin);

		}

		std::ostringstream playerHealthString, gameTimeString;    // объявили переменную здоровья и времени
		playerHealthString << p.health; gameTimeString << gameTime;		//формируем строку
		text.setString("Здоровье: " + playerHealthString.str() + "\nВремя игры: " + gameTimeString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str()
		text.setPosition(view.getCenter().x -300 , view.getCenter().y - 230);//задаем позицию текста, отступая от центра камеры
		window.draw(text);
		window.draw(p.sprite);
		window.draw(h.spriteHead);
		window.display();
	}


	return 0;
}
