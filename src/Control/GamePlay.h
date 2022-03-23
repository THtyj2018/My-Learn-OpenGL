#pragma once

class GamePlay
{
	friend class Application;

protected:
	GamePlay();

	virtual ~GamePlay();

	virtual int Initialize();

	virtual void Finalize();

	virtual void Update(float dt);

	virtual void Draw();

	virtual void OnSize(int width, int height);

	virtual void OnMouseMove(float x, float y);

private:
};