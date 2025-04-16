#include "Gumb.h"
#include "Globals.h"

Gumb::Gumb()
{
	m_Animation = new Animation();
	Pos.X = 840;
	Pos.Y = 450;

	width = 64;
	height = 64;

	scale = 3;

	hoovered = 0;
}

Gumb::Gumb(int x, int y, int w, int h, std::string id)
{
	m_Animation = new Animation();
	Pos.X = x;
	Pos.Y = y;

	width = w;
	height = h;

	this->id = id;

	hoovered = 0;
}


void Gumb::IsOver()
{
	SDL_GetMouseState(&mouseX, &mouseY);

	if (id == "gumb") {
		if (mouseX > Pos.X && mouseX < Pos.X + width * 2 && mouseY > Pos.Y && mouseY < Pos.Y + height * 2) {
			m_Animation->SetProps(id, 1, 1, 120, 2);
			hoovered = 1;
		}
		else {
			m_Animation->SetProps(id, 0, 6, 120, 2);
			hoovered = 0;
		}
	}

	else if (id == "carGumb" || id == "tankGumb" || id == "spominGumb" || id == "skinGumb") {
		
		if (mouseX > Pos.X && mouseX < Pos.X + width * scale && mouseY > Pos.Y && mouseY < Pos.Y + height * scale) {
			if (hoovered != 1) {
				Pos.Y -= 20;
				Pos.X -= 20;
				
			}
			scale = 3;
			m_Animation->SetProps(id, 0, 1, 120, scale);
			hoovered = 1;
		}
		else {
			if (hoovered == 1) {
				Pos.Y += 20;
				Pos.X += 20;
			}
			scale = 2;
			m_Animation->SetProps(id, 0, 1, 120, 2);
			hoovered = 0;
		}
			
	}

	else if (id == "exit") {

		if (mouseX > Pos.X && mouseX < Pos.X + width * scale && mouseY > Pos.Y && mouseY < Pos.Y + height * scale) {
			if (hoovered != 1) {
				Pos.Y -= 10;
				Pos.X -= 10;

			}
			scale = 3;
			m_Animation->SetProps(id, 0, 1, 120, scale);
			hoovered = 1;
		}
		else {
			if (hoovered == 1) {
				Pos.Y += 10;
				Pos.X += 10;
			}
			scale = 2;
			m_Animation->SetProps(id, 0, 1, 120, 2);
			hoovered = 0;
		}

	}

	else if (id == "tankSkin" || id == "avtoSkin") {

		if (mouseX > Pos.X && mouseX < Pos.X + width * scale && mouseY > Pos.Y && mouseY < Pos.Y + height * scale) {
			if (hoovered != 1) {
				Pos.Y -= 3;
				Pos.X -= 10;

			}
			scale = 3;
			m_Animation->SetProps(id, 0, 1, 120, 3);
			hoovered = 1;
		}
		else {
			if (hoovered == 1) {
				Pos.Y += 3;
				Pos.X += 10;
			}
			scale = 2;
			m_Animation->SetProps(id, 0, 1, 120, 2);
			hoovered = 0;
		}

	}


	else if (id == "okvir" || id == "rdec_okvir") {

		if (mouseX > Pos.X && mouseX < Pos.X + width && mouseY > Pos.Y && mouseY < Pos.Y + height) {
			hoovered = 1;
		}
		else {
			hoovered = 0;
		}

	}

	else if (id == "ustvari" || id == "prijava") {
		if (mouseX > Pos.X && mouseX < Pos.X + width * scale && mouseY > Pos.Y && mouseY < Pos.Y + height * scale) {
			if (hoovered != 1) {
				Pos.Y -= 3;
				Pos.X -= 10;

			}
			scale = 1;
			m_Animation->SetProps(id, 0, 1, 120, 1);
			hoovered = 1;
		}
		else {
			if (hoovered == 1) {
				Pos.Y += 3;
				Pos.X += 10;
			}
			scale = 0.7;
			m_Animation->SetProps(id, 0, 1, 120, 0.7);
			hoovered = 0;
		}
	}

	else {
		if (mouseX > Pos.X && mouseX < Pos.X + width * scale && mouseY > Pos.Y && mouseY < Pos.Y + height * scale) {
			scale = 2;
			m_Animation->SetProps(id, 0, 1, 120, scale);
			hoovered = 1;
		
		}
		else {
			scale = 1;
			m_Animation->SetProps(id, 0, 1, 120, 1);
			hoovered = 0;
		}
		TextureManager::GetInstance()->Draw("skinBorderGray", Pos.X - 2, Pos.Y - 2, width + 4, height + 4);
	}



	m_Animation->Update();

}

void Gumb::Draw()
{	
		
		if(id != "okvir" && id != "rdec_okvir")
			m_Animation->Draw(Pos.X, Pos.Y, width, height);

		else {
			TextureManager::GetInstance()->Draw(id, Pos.X, Pos.Y, width, height);
		}
}

void Gumb::FillCarVector()
{
	int x = 300;
	int y = 400;
	std::string _id;
	for (int i = 0; i < 8; i++) {
		if (i == 0)
			_id = "red";
		if (i == 1)
			_id = "blue";
		if (i == 2)
			_id = "yellow";
		if (i == 3)
			_id = "white";
		if (i == 4)
			_id = "orange";
		if (i == 5)
			_id = "green";
		if (i == 6)
			_id = "black";
		if (i == 7)
			_id = "slo";

		Gumb temp(x,y,64,64,_id);
		AvtoSkin.push_back(temp);
		x = x + 64 + 60;
	}

}

void Gumb::FillTankVector()
{
	int x = 300;
	int y = 400;
	std::string _id;
	for (int i = 0; i < 8; i++) {
		if (i == 0)
			_id = "blue";
		if (i == 1)
			_id = "greenM";
		if (i == 2)
			_id = "red";
		if (i == 3)
			_id = "lightgreen";
		if (i == 4)
			_id = "orangeM";
		if (i == 5)
			_id = "whiteM";
		if (i == 6)
			_id = "brownM";
		if (i == 7)
			_id = "military";

		Gumb temp(x, y, 64, 64, _id);
		TankSkin.push_back(temp);
		x = x + 64 + 60;
	}

}

void Gumb::DrawAvtoSkini()
{
	for (int i = 0; i < AvtoSkin.size(); i++) {
		TextureManager::GetInstance()->Draw(AvtoSkin[i].id, AvtoSkin[i].Pos.X, AvtoSkin[i].Pos.Y, AvtoSkin[i].width, AvtoSkin[i].height);
		if (AvtoSkin[i].hoovered == 1) {
			TextureManager::GetInstance()->Draw("skinBorderGray", AvtoSkin[i].Pos.X - 2, AvtoSkin[i].Pos.Y - 2, AvtoSkin[i].width + 4, AvtoSkin[i].height + 4);
		}
		if (AvtoSkin[i].id == carSkin) {
			TextureManager::GetInstance()->Draw("skinBorder", AvtoSkin[i].Pos.X - 2, AvtoSkin[i].Pos.Y - 2, AvtoSkin[i].width + 4, AvtoSkin[i].height + 4);
		}
	}
}

void Gumb::DrawTankSkini()
{
	for (int i = 0; i < TankSkin.size(); i++) {
		TextureManager::GetInstance()->Draw(TankSkin[i].id, TankSkin[i].Pos.X, TankSkin[i].Pos.Y, TankSkin[i].width, TankSkin[i].height);
		if (TankSkin[i].hoovered == 1) {
			TextureManager::GetInstance()->Draw("skinBorderGray", TankSkin[i].Pos.X - 2, TankSkin[i].Pos.Y - 2, TankSkin[i].width + 4, TankSkin[i].height + 4);
		}
		if (TankSkin[i].id == tankSkin) {
			TextureManager::GetInstance()->Draw("skinBorder", TankSkin[i].Pos.X - 2, TankSkin[i].Pos.Y - 2, TankSkin[i].width + 4, TankSkin[i].height + 4);
		}
	}
}


