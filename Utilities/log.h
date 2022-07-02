#ifndef _LOGGING_HPP
#define _LOGGING_HPP
#pragma once

/* Includes for this header file */
#include <cstdio>
#include <string>
#include <ctime>
#include <iostream>
#include <Windows.h>
ImFont *font;

/* Aura made Kiwans life easier now he makes my life easier */
#define SetTextColour(colour) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);

namespace Log
{
	/*
	* Used to write text with the given prefix and colour
	*/
	template <typename ... args>
	void LogWithColouredPrefix(bool new_line, std::string prefix, int colour, std::string str, args ...arguments)
	{
		/* Begin the log with the first bracket */
		printf("[");

		/* Update the prefix colour */
		SetTextColour(colour);

		/* Append the prefix to the log */
		printf(prefix.c_str());

		/* Reset the text colour */
		SetTextColour(7);

		/* Close the prefix bracket */
		printf("]: ");

		/* Check if we want a new line */
		if (new_line) str += "\n";

		/* Append the text */
		printf(str.c_str(), arguments...);
	}

	/* Used to log with the prefix [INFO] prefix */
	template <typename ... args>
	void Info(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(true, "INFO", 10, str, arguments...);
	}

	/* Used to log with the prefix [ERROR] prefix */
	template <typename ... args>
	void Error(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(true, "ERROR", 12, str, arguments...);
	}
	template <typename ... args>
	void Hooking( const std::string &str, args ...arguments )
	{
		LogWithColouredPrefix( true, "HOOK", 12, str, arguments... );
	}
	/* Used to log with the prefix [ERROR] prefix */
	template <typename ... args>
	void DirectX(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(true, "DIRECTX", 14, str, arguments...);
	}
	template <typename ... args>
	void ImGui( const std::string &str, args ...arguments )
	{
		LogWithColouredPrefix( true, "IMGUI", 10, str, arguments... );
	}

	/* Used to log with the prefix [ERROR] prefix */
	template <typename ... args>
	void Warn(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(true, "WARNING", 14, str, arguments...);
	}

	/* Used to log with the prefix [DUBUG] prefix */
	template <typename ... args>
	void Debug(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(true, "DEBUG", 14, str, arguments...);
	}

	/* Used to log with the prefix [Success] prefix */
	template <typename ... args>
	void Success(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(true, "SUCCESS", 10, str, arguments...);
	}

	/*Displays Sucess if its not null and Error if null*/
	template <class T>
	void Value(const std::string &str, T Value)
	{
		return;
		if (!Value)
		{
			Error(str, Value);
			//#ifdef DEBUG
			//			std::cin.get();
			//			exit(1);
			//#endif // !_DEBUG
		}
		else
			Success(str, Value);
	}

	template <typename ... args>
	void Initialize(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(true, "Initializing", 14, str, arguments...);
	}

	/* Used to gather input from a user */
	template <typename ... args>
	std::string Input(const std::string &str, args ...arguments)
	{
		LogWithColouredPrefix(false, "INPUT", 9, str, arguments...);

		/* Initialize the buffer for cin */
		std::string input;

		/* Get the user input */
		std::cin >> input;

		/* Return the input */
		return input;
	}


}

#endif // _LOGGING_HPP