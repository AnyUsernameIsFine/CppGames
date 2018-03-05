#pragma once

#include "Graphics\Graphics.h"
#include "Input\Input.h"

namespace Framework
{
	/// <summary>
	/// The Game class is what drives the application. The application project
	/// should create a class that inherits from it and call the 'run' method.
	/// The protected methods can be implemented by the inheriting class for
	/// the game logic, such as handling input and drawing the game world.
	/// </summary>
	class Game abstract
	{
	private:
		struct SDL
		{
			SDL();
			~SDL();
		};
		
		SDL sdl;

	public:
		/// <summary>
		/// Runs the game.
		/// </summary>
		/// <returns>
		/// 0 on succesful termination, 1 on failure
		/// </returns>
		int run();

		/// <summary>
		/// Returns the time in seconds since the start of the game.
		/// </summary>
		/// <returns>
		/// The time in seconds since the start of the game
		/// </returns>
		float getGameTimeInSeconds() const;

	protected:
		Graphics graphics;
		Input input;

		/// <summary>
		/// Is run once just before the game's window is opened.
		/// </summary>
		virtual void initialize() {}

		/// <summary>
		/// Is run when game logic needs to be processed.
		/// </summary>
		/// <param name="deltaSeconds">
		/// The amount of seconds passed since the previous call to this function.
		/// </param>
		virtual void update(float deltaSeconds) {}

		/// <summary>
		/// Is run when a frame needs to be drawn.
		/// </summary>
		virtual void draw() {}

		/// <summary>
		/// Is run once just before the game is exited.
		/// </summary>
		virtual void finalize() {}

	private:
		static const int MILLISECONDS_YIELD = 10;

		bool isRunning = false;
		bool isWindowActive = true;
		TimePoint startTime;
		float updateTimeInSeconds;

		void gameLoop();
		void sleep(int milliseconds) const;

		static int sdlEventHandler(void* data, SDL_Event* event);
	};
}
