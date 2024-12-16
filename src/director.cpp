#include "director.hpp"

Director::Director() : WIDTH(1366), HEIGHT(768), fooDrawInstance(window)
{
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Furdom Kingdoms");
    window.setFramerateLimit(60);
    nextScene = SceneState::MENU;
    gameOver = false;
    drawPlayer = false;
    drawEnemies = false;
    transitioning = true;
    transitionState = TransitionState::LOADING;
    view.setSize(window.getSize().x * .7f, window.getSize().y * .7f);
    alpha = 0;
    fadeRectangle.setSize(sf::Vector2f(WIDTH, HEIGHT));
    fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    fadeRectangle.setOrigin(WIDTH / 2.f, HEIGHT / 2.f);
    initUI();

    if(!font.loadFromFile("Textures/font/menuFont.ttf"))
    {
        std::cerr << "Error al cargar la fuente del menú" << std::endl;
        return;
    }

    text.setFont(font);
    text.setString("Game Over");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::Red);
    text.setOutlineThickness(2.f);
    text.setOutlineColor(sf::Color::White);

    saveGame.setFont(font);
    saveGame.setString("Save Game");
    saveGame.setCharacterSize(30);
    saveGame.setFillColor(sf::Color::White);

    newSong("Textures/menu/menuSoundtrack.mp3");
    music.setVolume(0.1);
}

void Director::run() ///buclePrincipal();
{
    static bool wasWindowFocused;
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            if(gameOver && cooldownElapsed >= 3.f && event.type == sf::Event::KeyPressed)
            {
                transitioning = true;
                transitionState = TransitionState::LOADING;
                player->setHp(100.f);
            }

            if(window.hasFocus() != wasWindowFocused)
            {
                if(window.hasFocus())
                    music.play();
                else
                    music.pause();
                wasWindowFocused = window.hasFocus();
            }

            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

                if(saveGame.getGlobalBounds().contains(worldMousePos))
                {
                    saveProgress();
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        updateScene(deltaTime);
        if(!gameOver)
        {
            if(transitionState != TransitionState::FADINGOUT)
            {
                update(deltaTime);
                gameEvents();
            }
        }
        render();
    }
}

void Director::update(float deltaTime)
{
    if(drawPlayer && player != nullptr)
    {
        if(window.hasFocus())
        {
            player->timers(world, deltaTime);
            player->keyboardInput(world);
        }
    }
    if(drawEnemies)
    {
        for(auto it = enemies.begin(); it != enemies.end();)
        {
            Enemy* enemy = *it;

            enemy->logic(player);
            enemy->update(deltaTime);
            enemy->render(window);

            if(!enemy->getIsAlive())
            {
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void Director::updateScene(float deltaTime)
{
    if(!transitioning && currentScene != nullptr && currentScene->shouldTransition())
    {
        transitioning = true;
        nextScene = currentScene->nextSceneState();
        transitionState = TransitionState::FADINGOUT;
    }

    if(transitioning || gameOver)
    {
        switch(transitionState)
        {
            case TransitionState::FADINGOUT:
                fadeOut(deltaTime);
                break;
            case TransitionState::LOADING:
                switch(nextScene)
                {
                    case SceneState::MENU:
                        initMenuScene();
                        break;
                    case SceneState::HOUSE:
                        initHouseScene();
                        break;
                    case SceneState::CITY:
                        initCityScene();
                        break;
                    case SceneState::BAR:
                        initBarScene();
                        break;
                    case SceneState::FOREST:
                        initForestScene();
                        break;
                    case SceneState::CASTLE:
                        initCastleScene();
                        break;
                    case SceneState::DUNGEON:
                        initDungeonScene();
                        break;
                    case SceneState::TEST:
                        initTestScene();
                        break;
                    case SceneState::DEFAULT:
                        break;
                }
                transitionState = TransitionState::FADINGIN;
                break;
            case TransitionState::FADINGIN:
                fadeIn(deltaTime);
                break;
            case TransitionState::GAMEOVER:
                {
                    fadeOut(deltaTime);
                    if(cooldownElapsed <= 3.f) cooldownElapsed += deltaTime;
                }
                break;
            case TransitionState::NONE:
                break;
            default:
                break;
        }
    }
    else if(currentScene != nullptr)
    {
        if(drawPlayer) currentScene->updatePlayer(deltaTime, player->getPos(), player->getScale(), player->getPlayerState());
        if(currentScene->gameOverBool())
        {
            cooldownElapsed = 0.f;
            gameOver = true;
            transitionState = TransitionState::GAMEOVER;
        }
        currentScene->update(window, deltaTime);
    }
}

void Director::gameEvents()
{
    if(world != nullptr)
    {
        world->Step(timeStep, velocityIterations, positionIterations);
    }
}

void Director::render()
{
    window.clear();
    if(currentScene != nullptr)
    {
        currentScene->render(window);
    }

    if(drawPlayer && player != nullptr)
    {
        view.setCenter(player->getPos().x * PPM, (player->getPos().y * PPM) - 50);
        window.setView(view);

        screenTopLeft = window.mapPixelToCoords(sf::Vector2i(0, 0));
        container.setPosition(screenTopLeft.x + 10.f, screenTopLeft.y + 10.f);
        hpContainer.setPosition(screenTopLeft.x + 69.f, screenTopLeft.y + 27.f);
        hpBar.setPosition(screenTopLeft.x + 71.8f, screenTopLeft.y + 31.f);

        playerLevel.setString(std::to_string(player->getLevel()));

        float hpPercentage = player->getActualHp().y / player->getActualHp().x;
        hpPercentage = std::max(0.f, std::min(hpPercentage, 1.f));

        float scale = (hpPercentage > 0.f) ? (hpPercentage * 1.1f) : 0.f;

        playerLevel.setPosition(screenTopLeft.x + 27.f, screenTopLeft.y + 16.f);

        saveGame.setPosition(screenTopLeft.x + window.getSize().x / 2 + 120, screenTopLeft.y + 0.f);

        hpBar.setScale(std::min(scale, 1.1f), 1.f);

        window.draw(container);
        window.draw(hpBar);
        window.draw(hpContainer);
        window.draw(playerLevel);
        window.draw(saveGame);
    }

    if(world)
    {
        world->DebugDraw();
    }

    if(transitioning)
    {
        fadeRectangle.setPosition(view.getCenter().x, view.getCenter().y - 100.f);
        window.draw(fadeRectangle);
    }

    if(gameOver)
    {
        fadeRectangle.setPosition(view.getCenter());
        text.setPosition(view.getCenter().x - (text.getLocalBounds().width / 2.f), view.getCenter().y - text.getLocalBounds().height);
        window.draw(fadeRectangle);
        window.draw(text);
    }

    window.display();
}

void Director::initMenuScene()
{
    if(world != nullptr)
    {
        delete world;
        world = nullptr;
    }
    gameOver = false;
    drawPlayer = false;
    drawEnemies = true;

    setScene(new MenuScene);

    newSong("Textures/menu/menuSoundtrack.mp3");
}

void Director::initHouseScene()
{
    gameOver = false;

    initWorld(10.f);

    setScene(new HouseScene);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 1000.f, 670.f, 100.f, 50.f, Kind::HOUSESENSOR);

    sensor->addObserver(currentScene);

    boundFactory->createBound(world, 576.f, 712.f, 221.f, 8.f, Kind::FLOOR);

    drawPlayer = true;

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createWall(world, 361.f, 656.f, 8.f, 64.f, Kind::WALLS);

    boundFactory->createWall(world, 791.f, 622.f, 8.f, 32.f, Kind::WALLS);

    boundFactory->createBound(world, 577.f, 712.f, 223.f, 8.f, Kind::LIMITS);

    boundFactory->createWall(world, 576.f, 560.f, 272.f, 32.f, Kind::LIMITS);

    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);

    player = new Player();
    player->createPlayer(world, 400.f, 680.f);
}

void Director::initCityScene()
{
    gameOver = false;

    initWorld(10.f);

    boundFactory = std::make_unique<SensorFactory>();
    sensor = boundFactory->createBound(world, 1370.f, 670.f, 5.f, 100.f, Kind::NEXTSCENE);

    setScene(new CityScene);
    sensor->addObserver(currentScene);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 400.f, 672.f, 20.f, 28.f, Kind::BARDOOR);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 100.f, 100.f, 100.f, 100.f, Kind::LIMITS);

    boundFactory->createBound(world, 666.f, 704.f, 1000.f, 0.f, Kind::FLOOR);

    drawPlayer = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(0.f / 30.f, 676.f / 30.f), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 100.f, 680.f);
    }

    sensor->addObserver(currentScene);
}

void Director::initBarScene()
{
    gameOver = false;

    initWorld(10.f);

    setScene(new BarScene);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 50.f, 672.f, 20.f, 28.f, Kind::BARDOOR);
    sensor->addObserver(currentScene);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 100.f, 695.f, 798.f, 1.f, Kind::FLOOR);

    boundFactory->createWall(world, 695.f, 630.f, 8.f, 80.f, Kind::WALLS);

    boundFactory->createWall(world, 50.f, 570.f, 8.f, 65.f, Kind::WALLS);

    boundFactory->createWall(world, 500.f, 540.f, 230.f, 10.f, Kind::WALLS);

    boundFactory->createWall(world, 16.f, 630.f, 8.f, 80.f, Kind::WALLS);

    boundFactory = std::make_unique<SensorFactory>();
    sensor = boundFactory->createBound(world, 280.f, 672.f, 20.f, 28.f, Kind::ARCADE);

    drawPlayer = true;


    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(50.f / PPM, 670.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 50.f, 670.f);
    }
}

void Director::initForestScene()
{
    gameOver = false;

    initWorld(10.f);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 2999.f, 688.f, 40.f,2.f, Kind::ADUNGEON);

    setScene(new ForestScene);

    sensor->addObserver(currentScene);

    drawPlayer = true;
    drawEnemies = true;

    enemyFactory = std::make_unique<GhostFactory>();

    enemies.push_back(enemyFactory->createEnemy(world,432.f, 500.f));
    enemies.push_back(enemyFactory->createEnemy(world,654.f, 500.f));
    enemies.push_back(enemyFactory->createEnemy(world, 1800.f, 560.f));

    enemyFactory = std::make_unique<GoblinFactory>();

    enemies.push_back(enemyFactory->createEnemy(world,878.f, 688.f ));
    enemies.push_back(enemyFactory->createEnemy(world,1609.f, 583 ));
    enemies.push_back(enemyFactory->createEnemy(world,1930.f, 599.f ));
    enemies.push_back(enemyFactory->createEnemy(world,2582.f, 523.f ));


    boundFactory = std::make_unique<SensorFactory>();
//    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);
    boundFactory->createBound(world, 40.f, 590.f, 39.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 120.f, 670.f, 39.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 185.f, 575.f, 25.f, 2.f, Kind::FLOOR);

    boundFactory->createBound(world, 263.f, 573.f, 22.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 320.f, 540.f, 13.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 432.f, 525.f, 77.f, 2.f, Kind::FLOOR);

    boundFactory->createBound(world, 327.f, 732.f, 39.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 386.f, 690.f, 49.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 483.f, 642.f, 49.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 592.f, 560.f, 64.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 654.f, 624.f, 64.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 878.f, 687.f, 109.f, 3.f, Kind::FLOOR);
    boundFactory->createBound(world, 850.f, 448.f, 32.f, 3.f, Kind::FLOOR);
    boundFactory->createBound(world, 824.f, 528.f, 87.f, 3.f, Kind::FLOOR);
    boundFactory->createBound(world, 995.f, 655.f, 34.f, 3.f, Kind::FLOOR);
    boundFactory->createBound(world, 1080.f, 623.f, 72.f, 3.f, Kind::FLOOR);

    boundFactory->createBound(world, 1192.f, 585.f, 22.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1270.f, 600.f, 37.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1328.f, 555.f, 13.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1362.f, 523.f, 13.f, 2.f, Kind::FLOOR);

    boundFactory->createBound(world, 1464.f, 480.f, 71.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1583.f, 496.f, 31.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1609.f, 560.f, 38.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1745.f, 590.f, 143.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1865.f, 560.f, 24.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1930.f, 575.f, 24.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1993.f, 624.f, 23.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2048.f, 648.f, 14.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2159.f, 688.f, 208.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2391.f, 636.f, 23.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2593.f, 605.f, 145.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2505.f, 465.f, 40.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2546.f, 491.f, 12.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2662.f, 556.f, 21.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2582.f, 523.f, 21.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2768.f, 482.f, 13.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2768.f, 513.f, 46.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2870.f, 524.f, 53.f,2.f, Kind::FLOOR);

    boundFactory = std::make_unique<LimitsFactory>();
    boundFactory->createBound(world, 40.f, 650.f, 40.f, 59.f, Kind::WALLS);
    boundFactory->createBound(world, 120.f, 691.f, 40.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 185.f, 645.f, 25.f, 70.f, Kind::WALLS);


    boundFactory->createBound(world, 263.f, 573.f, 23.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 320.f, 540.f, 14.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 432.f, 525.f, 78.f, 2.f, Kind::WALLS);

    boundFactory->createBound(world, 323.f, 780.f, 40.f, 38.f, Kind::WALLS);
    boundFactory->createBound(world, 386.f, 768.f, 50.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 483.f, 725.f, 50.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 592.f, 636.f, 65.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 654.f, 715.f, 65.f, 90.f, Kind::WALLS);
    boundFactory->createBound(world, 878.f, 765.f, 110.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 850.f, 527.f, 33.f, 80.f, Kind::WALLS);
    boundFactory->createBound(world, 824.f, 543.f, 88.f, 17.f, Kind::WALLS);
    boundFactory->createBound(world, 995.f, 740.f, 35.f, 85.f, Kind::WALLS);
    boundFactory->createBound(world, 1080.f, 708.f, 73.f, 85.f, Kind::WALLS);

    boundFactory->createBound(world, 1192.f, 587.f, 23.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 1270.f, 603.f, 38.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 1328.f, 555.f, 14.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 1362.f, 523.f, 14.f, 2.f, Kind::WALLS);

    boundFactory->createBound(world, 1464.f, 498.f, 72.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 1583.f, 514.f, 32.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 1609.f, 579.f, 40.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 1745.f, 610.f, 145.f,20.f, Kind::WALLS);
    boundFactory->createBound(world, 1865.f, 597.f, 26.f,38.f, Kind::WALLS);
    boundFactory->createBound(world, 1930.f, 597.f, 25.f,23.f, Kind::WALLS);
    boundFactory->createBound(world, 1993.f, 636.f, 24.f,14.f, Kind::WALLS);
    boundFactory->createBound(world, 2048.f, 651.f, 15.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2159.f, 736.f, 208.f,50.f, Kind::WALLS);

    boundFactory->createBound(world, 2391.f, 636.f, 23.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2594.f, 629.f, 145.f,21.f, Kind::WALLS);
    boundFactory->createBound(world, 2505.f, 487.f, 40.f,24.f, Kind::WALLS);
    boundFactory->createBound(world, 2546.f, 491.f, 13.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2582.f, 523.f, 22.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2662.f, 556.f, 22.f,2.f, Kind::WALLS);

    boundFactory->createBound(world, 2768.f, 525.f, 15.f,45.f, Kind::WALLS);
    boundFactory->createBound(world, 2768.f, 525.f, 48.f,13.f, Kind::WALLS);
    boundFactory->createBound(world, 2870.f, 525.f, 55.f,2.f, Kind::WALLS);/*HECHO*/
    boundFactory->createBound(world, 2999.f, 729.f, 40.f,40.f, Kind::WALLS);


    boundFactory->createBound(world, 2585.f, 679.f, 40.f,25.f, Kind::WALLS);
    boundFactory->createBound(world, 2090.f, 425.f, 282.f,25.f, Kind::WALLS);
    boundFactory->createBound(world, 2088.f, 455.f, 233.f,25.f, Kind::WALLS);
    boundFactory->createBound(world, 2193.f, 525.f, 33.f,50.f, Kind::WALLS);
    boundFactory->createBound(world, 0.f, 679.f, 5.f,500.f, Kind::WALLS);
     boundFactory->createBound(world, 3008.f, 679.f, 5.f,500.f, Kind::WALLS);

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(50.f / PPM, 500.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 10.f, 500.f);

    }
    currentScene->setEnemySprites(enemies);
}

void Director::initDungeonScene()
{
    gameOver = false;
    drawEnemies = true;

    initWorld(10.f);

    setScene(new DungeonScene);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 106.f, 858.f, 150.f, 88.f, Kind::WALLS);
    boundFactory->createBound(world, 290.f, 986.f, 150.f, 88.f, Kind::WALLS);
    boundFactory->createBound(world, 630.f, 955.f, 200.f, 120.f, Kind::WALLS);
//    boundFactory->createBound(world, 650.f, 920.f, 200.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1056.f, 920.f, 256.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1391.f, 987.f, 80.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1565.f, 1083.f, 100.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1687.f, 1147.f, 23.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1736.f, 1049.f, 25.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2055.f, 1177.f, 121.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2265.f, 1033.f, 25.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2671.f, 1033.f, 321.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3016.f, 1098.f, 25.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3220.f, 1033.f, 180.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3496.f, 1018.f, 88.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4020.f, 1115.f, 440.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4584.f, 1018.f, 136.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4584.f, 1018.f, 136.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4790.f, 917.f, 40.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 4880.f, 885.f, 34.f, 5.f, Kind::WALLS);

    boundFactory->createBound(world, 4960.f, 852.f, 34.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5055.f, 820.f, 50.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5167.f, 998.f, 33.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5240.f, 932.f, 25.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5328.f, 885.f, 48.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5407.f, 835.f, 32.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5489.f, 868.f, 33.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5489.f, 868.f, 33.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5655.f, 820.f, 137.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 6007.f, 1095.f, 137.f, 120.f, Kind::WALLS);

    boundFactory->createBound(world, 200.f, 582.f, 600.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1100.f, 615.f, 300.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 200.f, 582.f, 600.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1673.f, 663.f, 300.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2500.f, 663.f, 400.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1800.f, 663.f, 300.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2672.f, 730.f, 323.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2908.f, 700.f, 323.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3800.f, 558.f, 800.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4585.f, 710.f, 135.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 6007.f, 780.f, 137.f, 120.f, Kind::WALLS);

    boundFactory->createBound(world, 6007.f, 780.f, 5.f, 200.f, Kind::WALLS);
    boundFactory->createBound(world, -5.f, 780.f, 5.f, 200.f, Kind::WALLS);

    boundFactory = std::make_unique<SensorFactory>();
    boundFactory->createBound(world, 106.f, 770.f, 149.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 290.f, 898.f, 149.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 640.f, 832.f, 210.f, 3.f, Kind::FLOOR);
    boundFactory->createBound(world, 1056.f, 800.f, 255.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1391.f, 867.f, 79.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1565.f, 962.f, 99.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1687.f, 1027.f, 22.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1736.f, 929.f, 25.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2055.f, 1057.f, 120.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2265.f, 913.f, 24.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2671.f, 913.f, 320.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 3016.f, 978.f, 24.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 3220.f, 913.f, 179.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 3496.f, 897.f, 87.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4020.f, 994.f, 439.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4584.f, 897.f, 135.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4584.f, 897.f, 135.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4790.f, 913.f, 39.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4880.f, 879.f, 33.f, 3.f, Kind::FLOOR);
    boundFactory->createBound(world, 4880.f, 879.f, 33.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4960.f, 848.f, 33.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5055.f, 814.f, 49.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5167.f, 993.f, 32.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5240.f, 927.f, 24.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5328.f, 879.f, 47.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5407.f, 830.f, 31.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5489.f, 864.f, 32.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5489.f, 862.f, 32.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 5655.f, 815.f, 136.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 6007.f, 1090.f, 136.f, 120.f, Kind::FLOOR);

    enemyFactory = std::make_unique<GhostFactory>();

    enemies.push_back(enemyFactory->createEnemy(world, 200.f, 700.f));
    enemies.push_back(enemyFactory->createEnemy(world, 600.f, 700.f));
    enemies.push_back(enemyFactory->createEnemy(world, 1000.f, 700.f));
    enemies.push_back(enemyFactory->createEnemy(world, 1000.f, 700.f));
    enemyFactory = std::make_unique<GoblinFactory>();

    enemies.push_back(enemyFactory->createEnemy(world, 400.f, 780.f));

    enemyFactory = std::make_unique<SkeletonFactory>();
    enemies.push_back(enemyFactory->createEnemy(world, 1000.f, 700.f));

    enemyFactory = std::make_unique<GhostFactory>();

    enemies.push_back(enemyFactory->createEnemy(world,1056.f, 920.f));
    enemies.push_back(enemyFactory->createEnemy(world,1736.f, 1049.f));
    enemies.push_back(enemyFactory->createEnemy(world,4400.f, 850.f));

    enemyFactory = std::make_unique<SkeletonFactory>();

    enemies.push_back(enemyFactory->createEnemy(world,2671.f, 1033.f));

    enemyFactory = std::make_unique<GoblinFactory>();

    enemies.push_back(enemyFactory->createEnemy(world,4584.f, 1018.f ));
    enemies.push_back(enemyFactory->createEnemy(world,2159.f, 688.f ));
    enemies.push_back(enemyFactory->createEnemy(world,1930.f, 599.f ));
    enemies.push_back(enemyFactory->createEnemy(world,2582.f, 523.f ));
    enemies.push_back(enemyFactory->createEnemy(world,2594.f, 629.f));
    enemies.push_back(enemyFactory->createEnemy(world,4790.f, 913.f));


    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 5950.f, 920.f, 20.f, 50.f, Kind::CASTLE);

    sensor->addObserver(currentScene);

    drawPlayer = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(50.f / PPM, 670.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 50.f, 800.f);
    }
    currentScene->setEnemySprites(enemies);
}

void Director::initCastleScene()
{
    gameOver = false;

    initWorld(10.f);

    setScene(new CastleScene);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);
    boundFactory->createBound(world, 0.f, 370.f, 3000.f, 0.f, Kind::FLOOR);

    boundFactory->createBound(world, 400.f, 612.f, 80.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 400.f, 500.f, 80.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 720.f, 612.f, 65.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 720.f, 500.f, 65.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 720.f, 612.f, 65.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 720.f, 500.f, 65.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 1023.f, 612.f, 65.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 1023.f, 500.f, 65.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 1249.f, 612.f, 65.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 1249.f, 500.f, 65.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 1473.f, 612.f, 65.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 1473.f, 500.f, 65.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 1695.f, 612.f, 65.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 1695.f, 500.f, 65.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 1872.f, 500.f, 65.f, 5.f, Kind::FLOOR);
    boundFactory->createBound(world, 1857.f, 612.f, 65.f, 5.f, Kind::FLOOR);

    boundFactory->createBound(world, 320.f, 500.f, 2.f, 3000.f, Kind::WALLS);
    boundFactory->createBound(world, 1922.f, 500.f, 2.f, 3000.f, Kind::WALLS);

    boundFactory = std::make_unique<SensorFactory>();

    boundFactory->createBound(world, 40.f, 590.f, 39.f, 2.f, Kind::FLOOR);

    drawPlayer = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(400.f / PPM, 680.f / PPM), 0.0f);
    }
    else
    {
        player = new Player();
        player->createPlayer(world, 400.f, 680.f);
    }
}

void Director::initTestScene()
{
    gameOver = false;

    initWorld(9.8f);

    setScene(new TestScene);

    boundFactory = std::make_unique<SensorFactory>();

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 700.f, 700.f, 700.f, 1.f, Kind::FLOOR);

    enemyFactory = std::make_unique<GhostFactory>();

    //enemies.push_back(enemyFactory->createEnemy(world, 800.f, 700.f));

    enemyFactory = std::make_unique<SkeletonFactory>();

    enemies.push_back(enemyFactory->createEnemy(world, 600.f, 700.f));

    enemyFactory = std::make_unique<GoblinFactory>();

    //enemies.push_back(enemyFactory->createEnemy(world, 800.f, 700.f));

    drawPlayer = true;

    drawEnemies = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(4.f / PPM, 676.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 4.f, 676.f);
    }

    currentScene->setEnemySprites(enemies);
}

void Director::initUI()
{
    if(!uiFont.loadFromFile("Textures/font/uiFont.ttf"))
    {
        std::cerr << "Error al cargar la fuente de la UI" << std::endl;
        return;
    }

    playerLevel.setFont(uiFont);
    playerLevel.setCharacterSize(30.f);
    playerLevel.setFillColor(sf::Color::White);
    playerLevel.setOutlineThickness(2.f);
    playerLevel.setOutlineColor(sf::Color::Black);

    if(!containerText.loadFromFile("Textures/UI/container.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
        return;
    }
    if(!hpBarText.loadFromFile("Textures/UI/hpBar.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    if(!heartText.loadFromFile("Textures/UI/heart.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    if(!hpContainerText.loadFromFile("Textures/UI/hpContainer.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    container.setTexture(containerText);
    hpBar.setTexture(hpBarText);
    heart.setTexture(heartText);
    hpContainer.setTexture(hpContainerText);

    container.setScale(1.2f, 1.2f);

    hpContainer.setScale(1.f, 1.f);

    hpBar.setScale(1.1f, 1.f);
}

void Director::initWorld(float gravity)
{
    enemies.clear();

    if(world != nullptr)
    {
        cleanScene(world);
    }
    else
    {
        world = new b2World(b2Vec2(0.f, gravity));
    }

    if(colisionCheck == nullptr)
    {
        colisionCheck = new Colision();
    }

    world->SetContactListener(colisionCheck);

    fooDrawInstance.SetFlags(b2Draw::e_shapeBit);

    world->SetDebugDraw(&fooDrawInstance);
}

void Director::setScene(Scene* newScene)
{
    if(currentScene)
    {
        delete currentScene;
        currentScene = nullptr;
    }
    currentScene = newScene;
}

void Director::cleanScene(b2World* world)
{
    if(world != nullptr)
    {
        for(b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
        {
            UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

            if(body == player->getBody())
            {
                continue;
            }
            else
            {
                delete tag;
                world->DestroyBody(body);
            }
        }
    }
}

void Director::fadeOut(float deltaTime)
{
    if(alpha < 255)
    {
        alpha += 200 * deltaTime;
        if(alpha > 255)
        {
            alpha = 255;
        }
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        if(gameOver)
        {
            return;
        }
        transitionState = TransitionState::LOADING;
    }
}

void Director::fadeIn(float deltaTime)
{
    if(alpha > 0)
    {
        alpha -= 200 * deltaTime;
        if(alpha < 0)
        {
            alpha = 0;
        }
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        transitionState = TransitionState::NONE;
        transitioning = false;
    }
}

void Director::newSong(std::string path)
{
    music.stop();

    if(!music.openFromFile(path))
    {
        std::cerr << "Error al cargar la musica" << std::endl;
        return;
    }

    music.setLoop(true);
    music.play();
}

void Director::saveProgress()
{
    FILE* gameData = fopen("gameData.dat", "wb");
    if(gameData != nullptr)
    {
        int level = player->getLevel();
        fwrite(&level, sizeof(level), 1, gameData);

        SceneState lastScene = currentScene->getCurrentScene();
        int sceneValue = static_cast<int>(lastScene);
        fwrite(&sceneValue, sizeof(sceneValue), 1, gameData);

        fclose(gameData);
    }
    else
    {
        std::cerr << "Error al abrir el archivo de registro" << std::endl;
    }
}

Director::~Director()
{

    if(world != nullptr)
    {
        if(world->GetBodyCount() != 0)
        {
            for(b2Body* body = world->GetBodyList(); body != nullptr;body = body->GetNext())
            {
                UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

                delete tag;

                world->DestroyBody(body);
            }
            delete world;
        }
        world = nullptr;
    }
    if(player != nullptr)
    {
        delete player;
        player = nullptr;
    }
    if(sensor != nullptr)
    {
        delete sensor;
        sensor = nullptr;
    }
    if(currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
    }
    if(colisionCheck != nullptr)
    {
        delete colisionCheck;
        colisionCheck = nullptr;
    }
}
