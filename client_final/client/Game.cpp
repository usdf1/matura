#include "Game.h"
#include "TextureManager.h"
#include "client.h"
#include "Keyboard.h"
#include "Player.h"
#include "volan.h"
#include "Gumb.h"
#include "Collider.h"
#include "Number.h"
#include "SDL_ttf.h"
#include <cmath>
#include "Magg.h"
#include "Spomin_Polje.h"
#include <regex>
#include <iomanip>
#include "Servis.h"

NetworkClient network("77.38.88.66", 1234);
Game* Game::s_instance = nullptr;
Player player;
Volan volan;
Gumb gumb (840, 450, 64,64, "gumb");
Gumb carGumb(100, 100, 64, 64, "carGumb");
Gumb tankGumb(300, 100, 64, 64, "tankGumb");
Gumb spominGumb(500, 100, 64, 64, "spominGumb");
Gumb signUp(1000, 450, 128, 64, "ustvari");
Gumb logIn(1000, 550, 128, 64, "prijava");
Gumb username(SCREEN_WIDTH/2.5, SCREEN_HEIGHT/3, 500, 60, "okvir");
Gumb password(SCREEN_WIDTH / 2.5, SCREEN_HEIGHT / 3 + 130, 300, 60, "okvir");
Gumb submit(800, 600, 64, 64, "gumb");
Gumb skin(400, 200, 52, 20, "avtoSkin");
Gumb skin2(600, 200, 52, 20, "tankSkin");
Gumb exit1(1600, 100, 32, 32, "exit");
Gumb exit2(1400, 100, 32, 32, "exit");
Gumb back(700, 700, 32, 32, "exit");
Gumb skinGumb(40, 800, 64, 64, "skinGumb");
Collider collider;
Number number(650,420, 500, 200);
Text text;
Text text2;
Text text3;
Text text4;
Text text5;

Gumb AvtoSkini;
Gumb TankSkini;

Mag mag;

Polje board;
Servis servis(884, 860, 150, 150);

bool Game::Init() {

		if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
			std::cout << "fail" << std::endl;
			return false;
		}

		if (TTF_Init() == -1) {
			std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
			return -1;
		}

		m_window = SDL_CreateWindow("igrca", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (m_window == NULL) {
			std::cout << "okno se ne prikaze" << std::endl;
			return false;
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (m_renderer == NULL) {
			std::cout << "render error" << std::endl;
			return false;
		}


		TextureManager::GetInstance()->Load("player", "assets/pita.png");
		TextureManager::GetInstance()->Load("player2", "assets/brian.png");
		TextureManager::GetInstance()->Load("volan", "assets/volan.png");
		TextureManager::GetInstance()->Load("gumb", "assets/gumbek.png");
		TextureManager::GetInstance()->Load("menu", "assets/arcade3.png");
		TextureManager::GetInstance()->Load("player1", "assets/player1.png");
		TextureManager::GetInstance()->Load("avto", "assets/player.png");
		TextureManager::GetInstance()->Load("map", "assets/map.png");
		TextureManager::GetInstance()->Load("semafor", "assets/semafor.png");
		TextureManager::GetInstance()->Load("lap", "assets/lap.png");
		TextureManager::GetInstance()->Load("Gamemenu", "assets/Gamemenu.png");
		TextureManager::GetInstance()->Load("carGumb", "assets/carGumb.png");
		TextureManager::GetInstance()->Load("okvir", "assets/Okvir.png");
		TextureManager::GetInstance()->Load("rdec_okvir", "assets/Okvir_rdec.png");
		TextureManager::GetInstance()->Load("tank", "assets/tank.png");
		TextureManager::GetInstance()->Load("barrel", "assets/barrel.png");
		TextureManager::GetInstance()->Load("tank_map", "assets/tank_map.png");
		TextureManager::GetInstance()->Load("scit", "assets/shield.png");
		TextureManager::GetInstance()->Load("bullet", "assets/bullet.png");
		TextureManager::GetInstance()->Load("bullet_fire", "assets/bullet_fire.png");
		TextureManager::GetInstance()->Load("box", "assets/box.png");
		TextureManager::GetInstance()->Load("tankGumb", "assets/tankGumb.png");
		TextureManager::GetInstance()->Load("spomin_back", "assets/spomin_back.png");
		TextureManager::GetInstance()->Load("spomin1", "assets/spomin1.png");
		TextureManager::GetInstance()->Load("spomin2", "assets/spomin2.png");
		TextureManager::GetInstance()->Load("spomin3", "assets/spomin3.png");
		TextureManager::GetInstance()->Load("spomin4", "assets/spomin4.png");
		TextureManager::GetInstance()->Load("spomin5", "assets/spomin5.png");
		TextureManager::GetInstance()->Load("spomin6", "assets/spomin6.png");
		TextureManager::GetInstance()->Load("spomin0", "assets/spomin0.png");
		TextureManager::GetInstance()->Load("spomin7", "assets/spomin7.png");
		TextureManager::GetInstance()->Load("spomin8", "assets/spomin8.png");
		TextureManager::GetInstance()->Load("spomin9", "assets/spomin9.png");
		TextureManager::GetInstance()->Load("spominGumb", "assets/spominGumb.png");
		TextureManager::GetInstance()->Load("exit", "assets/exit.png");
		TextureManager::GetInstance()->Load("damage", "assets/damage.png");
		TextureManager::GetInstance()->Load("red_damage", "assets/red_damage.png");
		TextureManager::GetInstance()->Load("damage_border", "assets/damage_border.png");
		TextureManager::GetInstance()->Load("red_damage_border", "assets/red_damage_border.png");
		TextureManager::GetInstance()->Load("red", "assets/red.png");
		TextureManager::GetInstance()->Load("blue", "assets/blue.png");
		TextureManager::GetInstance()->Load("green", "assets/green.png");
		TextureManager::GetInstance()->Load("yellow", "assets/yellow.png");
		TextureManager::GetInstance()->Load("orange", "assets/orange.png");
		TextureManager::GetInstance()->Load("black", "assets/black.png");
		TextureManager::GetInstance()->Load("white", "assets/white.png");
		TextureManager::GetInstance()->Load("slo", "assets/slo.png");
		TextureManager::GetInstance()->Load("greenM", "assets/greenM.png");
		TextureManager::GetInstance()->Load("military", "assets/military.png");
		TextureManager::GetInstance()->Load("orangeM", "assets/orangeM.png");
		TextureManager::GetInstance()->Load("lightgreen", "assets/lightgreen.png");
		TextureManager::GetInstance()->Load("whiteM", "assets/whiteM.png");
		TextureManager::GetInstance()->Load("brownM", "assets/brownM.png");
		TextureManager::GetInstance()->Load("skinBorder", "assets/skinBorder.png");
		TextureManager::GetInstance()->Load("skinBorderGray", "assets/skinBorderGray.png");
		TextureManager::GetInstance()->Load("avtoSkin", "assets/avtoSkin.png");
		TextureManager::GetInstance()->Load("tankSkin", "assets/tankSkin.png");
		TextureManager::GetInstance()->Load("skinBackground", "assets/skinBackground.png");
		TextureManager::GetInstance()->Load("skinGumb", "assets/skinGumbReal.png");
		TextureManager::GetInstance()->Load("prijava", "assets/prijava.png");
		TextureManager::GetInstance()->Load("ustvari", "assets/ustvari.png");
		TextureManager::GetInstance()->Load("servis", "assets/servis.png");
		TextureManager::GetInstance()->Load("end", "assets/end.png");

		if (!text.loadFont("assets/BebasNeue-Regular.ttf", 32))
			std::cout << "balls";

		if (!text2.loadFont("assets/BebasNeue-Regular.ttf", 26))
			std::cout << "balls";

		if (!text3.loadFont("assets/BebasNeue-Regular.ttf", 52))
			std::cout << "balls";

		if (!text4.loadFont("assets/BebasNeue-Regular.ttf", 15))
			std::cout << "balls";

		if (!text5.loadFont("assets/STENCIL.ttf", 55))
			std::cout << "balls";

		collider.FillColliders();
		AvtoSkini.FillCarVector();
		TankSkini.FillTankVector();


		//board.FillVector();

		car_scale = 0.6; 
		tank_scale = 1.3;
		m_State = 0;
		return m_IsRunning = 1;
}



bool Game::Clean() {
	TextureManager::GetInstance()->Clean();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	IMG_Quit();
	SDL_Quit();
	return 1;
}


void Game::Update() {

	network.handleNetworkEvents(); 
	std::cout << players[currentPlayerId].CarS << "skin je tak" << std::endl;
	//Camera::GetInstance().SetTarget(player->GetOrigin());
	//Camera::GetInstance().Update(
	//std::cout << players[currentPlayerId].CarS << "aw" << std::endl;
	if (m_State == 0) {
		Exists = 0;
		gumb.IsOver();
		signUp.IsOver();
		logIn.IsOver();
	}

	else if (m_State == -1) {
		//std::cout << PLayersWaiting << "  na startig screeu" << std::endl;
		players[currentPlayerId].NumSkins();
		if (skinMenu == 0) {
			carGumb.IsOver();
			tankGumb.IsOver();
			spominGumb.IsOver();
			//skin.IsOver();
			//skin2.IsOver();
			exit1.IsOver();
			skinGumb.IsOver();

		}

		if (SpominStarted == 1) {

			PLayersWaitingForSpomin = 0;
			clickedSpomin = 0;
			m_State = 3;
			SpominStarted = 0;
			board.FillVector();
		}

		if (skinMenu == 1) {
			skin.IsOver();
			skin2.IsOver();
			exit2.IsOver();
			for(int i = 0; i < AvtoSkini.AvtoSkin.size(); i++)
				AvtoSkini.AvtoSkin[i].IsOver();
		}

		if (skinMenu == 2) {
			skin.IsOver();
			skin2.IsOver();
			exit2.IsOver();
			for (int i = 0; i < TankSkini.TankSkin.size(); i++)
				TankSkini.TankSkin[i].IsOver();
		}
		

	}

	else if (m_State == -2) {
		if (Exists == 1) {
			password.id = "rdec_okvir";
			username.id = "rdec_okvir";
		}
		if (Exists == 0) {
			password.id = "okvir";
			username.id = "okvir";
		}
		password.IsOver();
		username.IsOver();
		submit.IsOver();
		exit1.IsOver();
	}

	else if (m_State == -3) {

		if (Exists == 1) {
			password.id = "rdec_okvir";
			username.id = "rdec_okvir";
		}
		if (Exists == 0) {
			password.id = "okvir";
			username.id = "okvir";
		}

		password.IsOver();
		username.IsOver();
		submit.IsOver();
		exit1.IsOver();
	}


	else if (m_State == 1) {
		if (AvtiOver == 0) {
			if (!canMove) {
				if (SDL_GetTicks() - movementStartTime >= 5000) {
					canMove = true;
				}
			}
			Vector2D PrevPos = player.Position;

			if (canMove) {
				player.Update(collider);
				if (speed > 0 && damage < 5)
					player.angle = player.GetAngle(volan.angle);

			}
			int x = players[currentPlayerId].XPos();
			int y = players[currentPlayerId].YPos();
			network.sendPlayerPosition(currentPlayerId, x, y, player.angle, player.barrelAngle, player.shieldOn, m_State, players[currentPlayerId].CarS, players[currentPlayerId].TankS);
			if (player.laps == 3) {
				PLayersWaiting = 0;
				network.sendStop();
				damage = 0;
				player.StartPosition();
			}
			servis.CheckCollision();
		}

		if (AvtiOver == 1) {
			back.IsOver();
		}
	}

	else if (m_State == 2) {

		if (AvtiOver == 0) {
			player.Update(collider);
			player.barrelAngle = tankAngle;
			int x = players[currentPlayerId].XPos();
			int y = players[currentPlayerId].YPos();

			if (SDL_GetTicks() - LastBox >= BoxSpawnTime) {
				network.spawnBox(collider);
				LastBox = SDL_GetTicks();
			}


			mag.AddBullet();
			network.updateBullets(5);
			network.sendPlayerPosition(currentPlayerId, x, y, player.angle, player.barrelAngle, player.shieldOn, m_State, players[currentPlayerId].CarS, players[currentPlayerId].TankS);
			network.checkCollision(collider);
			if (network.CheckBoxes() == 1) {
				shield = 1;
				player.shieldOn = 1;
				powerUp = SDL_GetTicks();
			}

			if (network.CheckBoxes() == 2) {
				relode = 1;
				powerUp = SDL_GetTicks();
				mag.SetRelodeTime(500);
			}

			if (SDL_GetTicks() - powerUp >= 10000 && shield == 0) {
				mag.SetRelodeTime(2000);
				relode = 0;
			}

			if (SDL_GetTicks() - powerUp >= 10000 && shield == 1) {
				shield = 0;
				player.shieldOn = 0;
			}

			if (Is_Hit == 1) {
				network.sendStop();
			}
		}
		if (AvtiOver == 1) {
			back.IsOver();
		}
	}

	else if (m_State == 3) {
		if (AvtiOver == 0) {
			if (TurnEnded == 1 && YourTurn == 1) {
				TurnEnded = 0;
				network.sendEndTurn();
			}
			network.sendTocke();
			board.update();

			if (SpominTocka + NasprotnikTocke >= 10) {
				network.sendStop();
			}
		}
		if (AvtiOver == 1) {
			back.IsOver();
		}
	}
}



void Game::Render() {

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	SDL_RenderClear(m_renderer);
	//////////////////////

	if (m_State == 0) {
		TextureManager::GetInstance()->Draw("menu",0,0,1780,1060);
		//std::cout << carSkin << std::endl;
		gumb.Draw();
		signUp.Draw();
		logIn.Draw();
		if(PlayerNameReal != "")
		TextureManager::GetInstance()->renderText("Prijavljen Kot " + PlayerNameReal, { 255, 0, 255,255 }, text2, 1400, 100);
		//TextureManager::GetInstance()->renderText("Tocke" , { 0,0,0,255 }, text, 3, 0);   <- za text
	}

	else if (m_State == -1) {
		std::cout << carSkin << std::endl;
		TextureManager::GetInstance()->Draw("Gamemenu", 0, 0, 1780, 1060);
		carGumb.Draw();
		tankGumb.Draw();
		spominGumb.Draw();
		/*skin.Draw();
		skin2.Draw();*/
		skinGumb.Draw();
	if (skinGumb.hoovered == 1 && PlayerNameReal == "") {
		TextureManager::GetInstance()->renderText("NAJPREJ SE PRIJAVI", { 255,0,0,255 }, text4, skinGumb.Pos.X, skinGumb.Pos.Y + 50 + username.height + 4);
		//network.sendSkin();
	}
		if(skinMenu == 0)
			exit1.Draw();
		TextureManager::GetInstance()->renderText("[ " + std::to_string(PLayersWaiting) + " / 2 ]", { 0,0,0,255 }, text2, 100,  180);
		TextureManager::GetInstance()->renderText("[ " + std::to_string(PLayersWaitingForTanks) + " / 2 ]", { 0,0,0,255 }, text2, 300, 180);
		TextureManager::GetInstance()->renderText("[ " + std::to_string(PLayersWaitingForSpomin) + " / 2 ]", { 0,0,0,255 }, text2, 500, 180);

		if (skinMenu == 1) {
			TextureManager::GetInstance()->Draw("skinBackground", SCREEN_WIDTH/10, SCREEN_HEIGHT / 15, 1400, 800);

			skin.Draw();
			skin2.Draw();
			AvtoSkini.DrawAvtoSkini();
			exit2.Draw();
		}

		if (skinMenu == 2) {
			TextureManager::GetInstance()->Draw("skinBackground", SCREEN_WIDTH / 10, SCREEN_HEIGHT / 15, 1400, 800);
			skin.Draw();
			skin2.Draw();
			TankSkini.DrawTankSkini();
			exit2.Draw();
		}


	}

	else if (m_State == -2) {
		TextureManager::GetInstance()->Draw("Gamemenu", 0, 0, 1780, 1060);
		TextureManager::GetInstance()->renderText("USTVARI RACUN", {0,0,0,255}, text2, 800, 50);
		TextureManager::GetInstance()->renderText(" " + PlayerName, { 0, 0, 0,255 }, text2, username.Pos.X + 5, username.Pos.Y + 5);
		TextureManager::GetInstance()->renderText(" " + SecretPassword, { 0, 0, 0,255 }, text2, password.Pos.X + 5, password.Pos.Y + 5);
		exit1.Draw();
		if (IsNotEmail == 1) {
			TextureManager::GetInstance()->renderText("NI VALIDEN EMAIL", {255,0,0,255}, text4, username.Pos.X, username.Pos.Y + username.height + 4);
		}

		else if (zeObstaja == 1) {
			TextureManager::GetInstance()->renderText("UPORABNIK ZE OBSTAJA", { 255,0,0,255 }, text4, username.Pos.X, username.Pos.Y + username.height + 4);
		}
		password.Draw();
		username.Draw();
		submit.Draw();
	}

	else if (m_State == -3) {
		TextureManager::GetInstance()->Draw("Gamemenu", 0, 0, 1780, 1060);
		TextureManager::GetInstance()->renderText("PRIJAVA V RACUN", { 0,0,0,255 }, text2, 800, 50);
		TextureManager::GetInstance()->renderText(" " + PlayerName, { 0, 0, 0,255 }, text2, username.Pos.X + 5, username.Pos.Y + 5);
		TextureManager::GetInstance()->renderText(" " + SecretPassword, { 0, 0, 0,255 }, text2, password.Pos.X + 5, password.Pos.Y + 5);
		password.Draw();
		username.Draw();
		submit.Draw();
		exit1.Draw();
	}

	else if (m_State == 1) {
		//std::cout << carSkin << std::endl;
		TextureManager::GetInstance()->Draw("map", 0, 0, 1792, 1088);
		if (volan.ReturnIsHeld()) {
			volan.CheckForHold();
		}

		if (!canMove) {
			number.Draw(SDL_GetTicks() - movementStartTime);
		}

		volan.Draw();
		servis.Draw();
		network.renderPlayers();
		TextureManager::GetInstance()->renderText("Opravljeni Krogi", { 0,0,0,255 }, text, 70, 900);
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(1) << (speed * 10);
		std::string speedStr = stream.str();
		TextureManager::GetInstance()->renderText(speedStr + "KM/H", { 200,200,200,255 }, text5, 120, 30);

		if (player.laps > 0) {       //za narista zastave za laps-e
			for (int i = 0; i < player.laps; i++) {
				TextureManager::GetInstance()->Draw("lap", 30 + (i * 100), 940, 64, 64);
			}
		}
		
		//za damage bar
		int b_x = 60, b_y = 800, b_w = 200, b_h = 30;
		float clampedDamage = (damage < 0.0f) ? 0.0f : (damage > 5.0f ? 5.0f : damage);
		int currentBarWidth = static_cast<int>(b_w * (1.0f - (clampedDamage / 5.0f)));
		if (damage <= 3) {
			TextureManager::GetInstance()->renderText("Skoda na avtu:", { 255,255,255,255 }, text, b_x, b_y - 50);
			TextureManager::GetInstance()->Draw("damage", b_x, b_y, currentBarWidth, b_h);
			TextureManager::GetInstance()->Draw("damage_border", b_x - 5, b_y - 5, b_w + 10, b_h + 10);
		}

		else {
			TextureManager::GetInstance()->renderText("Skoda na avtu:", { 167,46,59,255 }, text, b_x, b_y - 50);
			TextureManager::GetInstance()->Draw("red_damage", b_x, b_y, currentBarWidth, b_h);
			TextureManager::GetInstance()->Draw("red_damage_border", b_x - 5, b_y - 5, b_w + 10, b_h + 10);
		}

		if (AvtiOver == 1) {
			TextureManager::GetInstance()->Draw("end", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			if(player.laps == 3) TextureManager::GetInstance()->renderText("ZMAGA!", { 0,255,0,255 }, text5, 650, 100);
			else TextureManager::GetInstance()->renderText("IZGUBIL SI", { 255,0,0,255 }, text5, 650, 100);
			back.Draw();
		}


		//collider.Draw();
	}

	else if (m_State == 2) {
		
		TextureManager::GetInstance()->Draw("tank_map", 0, 0, 1792, 1088);
		network.renderBullets();
		//collider.Draw();
		mag.Draw();
		network.renderTanks();
		network.renderBoxes();
		
		if (AvtiOver == 1) {
			TextureManager::GetInstance()->Draw("end", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (Is_Hit == 0) TextureManager::GetInstance()->renderText("ZMAGA!", { 0,255,0,255 }, text5, 650, 100);
			else TextureManager::GetInstance()->renderText("IZGUBIL SI", { 255,0,0,255 }, text5, 650, 100);
			back.Draw();
		}
	}

	else if (m_State == 3) {

		TextureManager::GetInstance()->Draw("Gamemenu", 0, 0, 1792, 1088);
		TextureManager::GetInstance()->renderText("TVOJE TOCKE:  " + std::to_string(SpominTocka), { 0,0,0,255 }, text3, 1000, 300);
		TextureManager::GetInstance()->renderText("NASPROTNIKOVE TOCKE:  " + std::to_string(NasprotnikTocke), { 175,20,100,255 }, text3, 1000, 400);
		board.render();

		if (AvtiOver == 1) {
			TextureManager::GetInstance()->Draw("end", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (SpominTocka > NasprotnikTocke) TextureManager::GetInstance()->renderText("ZMAGA!", { 0,255,0,255 }, text5, 650, 100);
			else if (SpominTocka < NasprotnikTocke) TextureManager::GetInstance()->renderText("IZGUBIL SI", { 255,0,0,255 }, text5, 650, 100);
			else TextureManager::GetInstance()->renderText("IZENACENO", { 0,0,255,255 }, text5, 650, 100);
			back.Draw();
		}
	}


	////////////////
		SDL_RenderPresent(m_renderer);

}



void Game::Events() {
	SDL_StartTextInput();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_IsRunning = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (GetState() == 0) {
					if (gumb.hoovered == 1) {
						m_State = -1;
						gumb.hoovered = 0;
					}

					else if (signUp.hoovered == 1) {
						submit.hoovered = 0;
						m_State = -2;
					}

					else if (logIn.hoovered == 1) {
						submit.hoovered = 0;
						m_State = -3;
					}
				}

				if (GetState() == -2) { // cretae an account
					if (password.hoovered == 1) {
						InPassword = 1;
						InUsername = 0;
					}

					else if (username.hoovered == 1) {
						InPassword = 0;
						InUsername = 1;
					}

					else if (submit.hoovered == 1) {
						//m_State = 0;
						submit.hoovered = 0;
						const std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
						if (std::regex_match(PlayerName, emailPattern)) {
							network.sendPlayerInfo(PlayerName, PlayerPassword); 
							Exists = 0;
							IsNotEmail = 0;
						}
							
						else {
							Exists = 1;
							IsNotEmail = 1;
							SecretPassword.clear();
						}
						PlayerPassword = "";
						PlayerName = "";
						
					}

					else if (exit1.hoovered == 1) {
						m_State = 0;
						InPassword = 0;
						InUsername = 0;
						PlayerPassword = "";
						PlayerName = "";
						exit1.hoovered = 0;
						exit1.Pos.Y += 10;
						exit1.Pos.X += 10;
					}

				}

				if (GetState() == -3) { // login
					if (password.hoovered == 1) {
						InPassword = 1;
						InUsername = 0;
					}

					else if (username.hoovered == 1) {
						InPassword = 0;
						InUsername = 1;
					}

					else if (submit.hoovered == 1) {
						submit.hoovered = 0;
						network.sendPlayerLogIn(PlayerName, PlayerPassword);
						//m_State = 0;
					}

					else if (exit1.hoovered == 1) {
						m_State = 0;
						InPassword = 0;
						InUsername = 0;
						PlayerPassword = "";
						PlayerName = "";
						exit1.hoovered = 0;
						exit1.Pos.Y += 10;
						exit1.Pos.X += 10;
					}
				}

				else if (GetState() == -1) {

					if (skinMenu == 0) {
						if (carGumb.hoovered == 1 /*&& clicked == 0*/) {
							clicked = 1;
							clickedTanks = 0;
							player.laps = 0;
							player.angle = 0;
							network.sendWait();
						}

						else if (tankGumb.hoovered == 1) {
							clickedTanks = 1;
							clicked = 0;
							player.angle = 0;
							mag.FillMagazine();
							network.sendTankWait();
						}
						else if (spominGumb.hoovered == 1) {
							clickedSpomin = 1;
							network.sendWaitSpomin();
						}

						else if (skinGumb.hoovered == 1 && PlayerNameReal != "") {
							skinMenu = 1;
							skin.hoovered == 0;
							//network.sendSkin();
						}


						//else if (skin2.hoovered == 1 && PlayerNameReal != "") {
						//	skinMenu = 2;
						//	skin2.hoovered == 0;
						//	//network.sendSkin();
						//}

						else if (exit1.hoovered == 1) {
							m_State = 0;
							exit1.hoovered = 0;
							exit1.Pos.Y += 10;
							exit1.Pos.X += 10;

						}
					}

					else if(skinMenu == 1){
						if (skin2.hoovered == 1 && PlayerNameReal != "") {
						skinMenu = 2;
						skin2.hoovered == 0;
						//network.sendSkin();
						}
						for (int i = 0; i < AvtoSkini.AvtoSkin.size(); i++) {
							if (AvtoSkini.AvtoSkin[i].hoovered == 1 && skinMenu == 1) {
								carSkin = AvtoSkini.AvtoSkin[i].id;
								AvtoSkini.AvtoSkin[i].hoovered = 0;
								network.sendSkin();
							}
						}
						 if (exit2.hoovered == 1) {
							skinMenu = 0;
							exit2.hoovered = 0;
							exit2.Pos.Y += 10;
							exit2.Pos.X += 10;

						 }

					}

					else if (skinMenu == 2) {
						if (skin.hoovered == 1 && PlayerNameReal != "") {
							skinMenu = 1;
							skin.hoovered == 0;
							//network.sendSkin();
						}

						for (int i = 0; i < TankSkini.TankSkin.size(); i++) {
							if (TankSkini.TankSkin[i].hoovered == 1 && skinMenu == 2) {
								tankSkin = TankSkini.TankSkin[i].id;
								TankSkini.TankSkin[i].hoovered = 0;
								network.sendSkin();
							}
						}
						if (exit2.hoovered == 1) {
							skinMenu = 0;
							exit2.hoovered = 0;
							exit2.Pos.Y += 10;
							exit2.Pos.X += 10;

						}

					}
				
				}

				else if (GetState() == 1) {
					if (AvtiOver == 1) {
						if (back.hoovered == 1) {
							AvtiOver = 0;
							m_State = -1;
							player.laps = 0;
						}
					}

				}

				else if (GetState() == 2) {
					/*int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);
					float dx = static_cast<float>(mouseX) - players[currentPlayerId].Position.X;
					float dy = static_cast<float>(mouseY) - players[currentPlayerId].Position.Y;
					tankAngle = std::atan2(dy, dx);*/
					if (mag.magazine.size() > 0 && mag.GetDelay()) {
						network.fireBullet();  // 1
						mag.relode = SDL_GetTicks();
						mag.SubBullet();
					}

					if (AvtiOver == 1) {
						if (back.hoovered == 1) {
							AvtiOver = 0;
							m_State = -1;
							Game::GetInstance()->Is_Hit = 0;
						}
					}
					
				}

				else if (GetState() == 3) {
					if (YourTurn == 1) {
						int x, y;
						SDL_GetMouseState(&x, &y);
						board.handleClick(x, y);
						network.sendTurnMsg();
					}
					if (AvtiOver == 1) {
						if (back.hoovered == 1) {
							AvtiOver = 0;
							m_State = -1;
							NasprotnikTocke = 0;
							SpominTocka = 0;
						}
					}
					
						
				}

				volan.OnMouseDown();
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				volan.OnMouseUp();
			}
			break;

		case SDL_TEXTINPUT:
			if(m_State == -2 || m_State == -3)
			{
				if(InUsername == 1)
				{
					if (PlayerName.size() <= 22)
						PlayerName += event.text.text;
				}
					
				else if (InPassword == 1) {
					if (PlayerPassword.size() <= 12) {
						PlayerPassword += event.text.text;
						SecretPassword += "*";
					}
						
				}
				break;
			}


		case SDL_KEYDOWN:
			if(m_State == -2 || m_State == -3)
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				if(InUsername == 1)
				{
					if (!PlayerName.empty()) {
						PlayerName.pop_back();
					}
				}
				else if (InPassword == 1)
				{
					if (!PlayerPassword.empty()) {
						PlayerPassword.pop_back();
						SecretPassword.pop_back();
					}
				}
			}

			break;
		}

	}
	//Keyboard::GetInstance()->Listen();
	/*SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		m_IsRunning = 0;
	default:
		break;
	}*/
	SDL_StopTextInput();
}


void Game::Quit() {
	Game::GetInstance()->Clean();
	m_IsRunning = 0;
	network.Disconnect();
}