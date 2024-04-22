
#include <SDL.h>
#include <SDL_ttf.h>
#include<Windows.h>
#include<conio.h>
#include<iostream>
#include<thread>
#include<chrono>
#include<vector>
#include<algorithm>
#include <queue>

#include"edge.h"
#include "networkFlowAnimation.h"
#include "maxNetworkFlow.h"

using namespace std;

void color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Edges {
	vector<Edge> edges;

	vector<pair<int, int>> vertices;

	int source = 0;
	int sink = 0;
	vector<int> best_path;

public:

	int maxVertex = -1;
	void edge_insertion() {
		edges.clear();
		vertices.clear();

		system("cls");

		cout << "\n\n\t\t\t\t[ DATA INSERTION ]" << endl;
		cout << "\t\t\t--------------------------------" << endl;


		int no_of_edges;
		cout << "How many edges or roads you want to connect between the source and sink: ";
		cin >> no_of_edges;

		for (int i = 0; i < no_of_edges; i++)
		{

			cout << "EDGE " << i + 1 << endl;

			int from = 0;
			int to = 0;
			int weight = 0;

			cout << "Enter from vertex: ";
			cin >> from;

			cout << "Enter to vertex: ";
			cin >> to;

			cout << "Enter weight of the edge: ";
			cin >> weight;

			edges.push_back({ from, to, weight });

		}

		system("cls");

		cout << "\n\n\t\t\t\t[ DATA INSERTION ]" << endl;
		cout << "\t\t\t--------------------------------" << endl;

		cout << "Now enter the source and sink: " << endl;
		cout << "(We recommend you to enter the source and sink as integers which you have mentioned in the previous from and to vertices)" << endl;;
		cout << "Source:";
		cin >> source;
		cout << "Sink: ";
		cin >> sink;
		int upSide = 1;
		int downSide = 1;

		for (const auto& edge : edges)
		{
			maxVertex = max(maxVertex, max(edge.from, edge.to));
		}

		for (int i = 0; i <= maxVertex; i++)
		{
			if (i == source)
			{
				vertices.push_back(make_pair(200, SCREEN_HEIGHT / 2));
			}

			else if (i == sink)
			{
				vertices.push_back(make_pair(SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2));
			}

			else if (i % 2 == 0) {
				vertices.push_back(make_pair((upSide * (SCREEN_WIDTH - 400) / (maxVertex - 1)) + 200, SCREEN_HEIGHT / 4));
				upSide++;
			}
			else
			{
				vertices.push_back(make_pair((downSide * (SCREEN_WIDTH - 400) / (maxVertex - 1)) + 200, 3 * SCREEN_HEIGHT / 4));
				downSide++;
			}
		}


		return;



	}

	void optimized_path()
	{

		system("cls");

		cout << "\n\n\t\t\t\t[ BEST WAY TO TRAVEL ]" << endl;
		cout << "\t\t\t--------------------------------" << endl;

		cout << "Below is the best path for the car to travel from the source to the sink: " << endl;;

		NetworkFlow flow;

		std::vector<std::vector<int>> capacity(vertices.size(), std::vector<int>(vertices.size(), 0));
		for (const auto& edge : edges) {
			capacity[edge.from][edge.to] = edge.weight;
		}

		best_path = flow.optimizedPath(edges, source, sink, vertices.size());

		for (int i = 0; i < best_path.size(); i++)
		{
			std::cout << best_path[i];
			if (i != best_path.size() - 1)
			{
				cout << "->";
			}
		}

		return;

	}


	void maxNetworkFlow() {
		system("cls");

		cout << "\n\n\t\t\t\t[ MAX FLOW OF CARS ]" << endl;
		cout << "\t\t\t--------------------------------" << endl;

		NetworkFlow flow;

		std::vector<std::vector<int>> capacity(vertices.size(), std::vector<int>(vertices.size(), 0));
		for (const auto& edge : edges) {
			capacity[edge.from][edge.to] = edge.weight;
		}

		int maxFlow = flow.fordFulkerson(capacity, source, sink);

		cout << endl;
		cout << "The maximum number of cars that can go through this network are : " << maxFlow;

		return;

	}


	int visualization() {
		NetworkFlow f;
		best_path = f.optimizedPath(edges, source, sink, vertices.size());

		system("cls");

		cout << "\n\n\t\t\t\t[ ROAD NETWORK FLOW ]" << endl;
		cout << "\t\t\t\t[ENTERING GRAPHICS MODE]" << endl;

		Visualization flow;

		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		if (!flow.init(&window, &renderer)) {
			std::cerr << "Failed to initialize!" << std::endl;
			return -1;
		}
		TTF_Font* font = flow.loadFont("C:\\Users\\nishc\\source\\repos\\Project1\\Project1\\Arial.ttf", 24);
		if (!font) {
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
			return -1;
		}

		const int BOX_SIZE = 10;
		std::vector<std::vector<int>> capacity(vertices.size(), std::vector<int>(vertices.size(), 0));
		for (const auto& edge : edges) {
			capacity[edge.from][edge.to] = edge.weight;
		}

		// Determine the edge with the minimum capacity in the best path
		int minCapacityEdge = -1;
		int minCapacity = 0;
		for (const auto& edge : edges)
		{
			if (minCapacity < edge.weight)
			{
				minCapacity = edge.weight;
			}
		}
		for (int i = 0; i < best_path.size() - 1; ++i) {
			int from = best_path[i];
			int to = best_path[i + 1];
			if (capacity[from][to] < minCapacity) {
				minCapacity = capacity[from][to];
				minCapacityEdge = i;
			}
		}

		float boxPosition = 0.0f;

		// Main loop flag
		bool quit = false;

		// Event handler
		SDL_Event e;

		// While application is running
		while (!quit) {
			// Handle events
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}

			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			// Draw edges
			for (const auto& edge : edges) {
				const auto& from = vertices[edge.from];
				const auto& to = vertices[edge.to];

				// Draw line
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Set edge color
				SDL_RenderDrawLine(renderer, from.first, from.second, to.first, to.second);

				// Calculate the position along the edge line at 2/3rd distance
				float arrowX = from.first + (2.0f / 3.0f) * (to.first - from.first);
				float arrowY = from.second + (2.0f / 3.0f) * (to.second - from.second);

				// Calculate the angle of the arrow
				float angle = atan2(to.second - from.second, to.first - from.first);
				// Length of the arrowhead lines
				float arrowLength = 15.0f;
				// Angle of the arrowhead lines relative to the edge line
				float arrowAngle = M_PI / 6; // 30 degrees

				// Calculate arrow points
				float arrowX1 = arrowX - arrowLength * cos(angle - arrowAngle);
				float arrowY1 = arrowY - arrowLength * sin(angle - arrowAngle);
				float arrowX2 = arrowX - arrowLength * cos(angle + arrowAngle);
				float arrowY2 = arrowY - arrowLength * sin(angle + arrowAngle);

				// Draw arrowhead lines
				SDL_RenderDrawLine(renderer, arrowX, arrowY, static_cast<int>(arrowX1), static_cast<int>(arrowY1));
				SDL_RenderDrawLine(renderer, arrowX, arrowY, static_cast<int>(arrowX2), static_cast<int>(arrowY2));

				// Calculate midpoint of edge for text placement
				int midX = (from.first + to.first) / 2;
				int midY = (from.second + to.second) / 2;
				std::string weightText = std::to_string(edge.weight);
				flow.renderText(renderer, font, weightText, midX, midY, { 0, 0, 0, 255 }); // Edge weight text
			}

			// Draw vertices
			for (size_t i = 0; i < vertices.size(); ++i) {
				int x = vertices[i].first - 15; // Adjust for visual centering
				int y = vertices[i].second - 15;
				if (i == source || i == sink) {
					SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // Set source/sink color
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Set vertex color
				}
				SDL_Rect fillRect = { x, y, 30, 30 };
				SDL_RenderFillRect(renderer, &fillRect);

				// Vertex number text
				std::string vertexNumber = std::to_string(i);
				flow.renderText(renderer, font, vertexNumber, x + 7, y + 5, { 255, 255, 255, 255 });
			}

			// Draw maximum flow path
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Set red color for path
			for (size_t i = 0; i < best_path.size() - 1; ++i) {
				const auto& from = vertices[best_path[i]];
				const auto& to = vertices[best_path[i + 1]];
				SDL_RenderDrawLine(renderer, from.first, from.second, to.first, to.second);
			}


			// Draw the moving box along the path from source to sink

				if (boxPosition < best_path.size() - 1) {
					// Calculate the position of the moving box along the path
					float boxX, boxY;

					const auto& from = vertices[best_path[static_cast<int>(boxPosition)]];
					const auto& to = vertices[best_path[static_cast<int>(boxPosition) + 1]];
					float deltaX = to.first - from.first;
					float deltaY = to.second - from.second;
					boxX = from.first + deltaX * (boxPosition - static_cast<int>(boxPosition));
					boxY = from.second + deltaY * (boxPosition - static_cast<int>(boxPosition));

					// Draw the moving box
					SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Set green color for the moving box
					SDL_Rect movingRect = { static_cast<int>(boxX), static_cast<int>(boxY), BOX_SIZE, BOX_SIZE };
					SDL_RenderFillRect(renderer, &movingRect);

					// Update box position for next frame
					boxPosition += 0.02f; // Adjust speed as needed

					// Reset box position when it reaches the end of the path
					if (boxPosition >= best_path.size() - 1) {
						boxPosition = best_path.size() - 1;
					}
				}
			



			// Update screen
			SDL_RenderPresent(renderer);

			SDL_Delay(1000 / 25); // Approximately 60 frames per second
		}

		// Free resources and close SDL
		TTF_CloseFont(font);
		font = nullptr;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();



		
	}

};

int main(int argc, char* argv[])
{
	int Set[] = { 10,10,10,10,10,10 };
	int counter = 1;
	char key;
	int i = 0;

	Edges a;

	while (1)
	{
		system("cls");

		color(10);
		cout << "\n\n\t\t\t\t[ MAIN MENU ]" << endl;
		cout << "\t\t\t-----------------------------" << endl;


		gotoxy(7, 8);
		color(Set[0]);
		cout << "1. Inputs for the road network flow" << endl;

		gotoxy(7, 9);
		color(Set[1]);
		cout << "2. Finding Best Way to Travel" << endl;

		gotoxy(7, 10);
		color(Set[2]);
		cout << "3. Max Cars to travel" << endl;

		gotoxy(7, 11);
		color(Set[3]);
		cout << "4. Road Network Flow" << endl;

		gotoxy(7, 14);
		color(Set[4]);
		cout << "0. Exit the program" << endl;

		gotoxy(20, 20);
		color(7);
		cout << "TIP:USE UP AND DOWN ARROW KEYS FOR NAVIGATION" << endl;

		key = _getch();

		if (key == 72 && (counter > 1 && counter <= 5)) {
			counter--;
			i--;
		}

		if (key == 80 && (counter >= 1 && counter < 5)) {
			counter++;
			i++;
		}

		for (int j = 0; j < 5; j++) {
			Set[j] = 10;
		}

		Set[i] = 12;

		if (key == '\r') {
			if (counter == 1) {
				
				a.edge_insertion();

				cout << "Returning to the menu..." << endl;
				this_thread::sleep_for(std::chrono::seconds(2));
			}

			if (counter == 2) {

				if (a.maxVertex > -1) {
					a.optimized_path();
					cout << endl;
					cout << "Returning to the menu..." << endl;
					this_thread::sleep_for(std::chrono::seconds(3));
				}

				else {
					system("cls");

					cout << "THERE IS NO GRAPH FOR THE INPUT, KINDLY GIVE US SOME INPUT :)" << endl;
					cout << "Returning to the menu..." << endl;
					this_thread::sleep_for(std::chrono::seconds(3));

				}



			}

			if (counter == 3) {

				if (a.maxVertex > -1)
				{
					a.maxNetworkFlow();
					cout << endl;
					cout << "Returning to the menu..." << endl;
					this_thread::sleep_for(std::chrono::seconds(3));

				}
				else {
					system("cls");

					cout << "THERE IS NO GRAPH FOR THE INPUT, KINDLY GIVE US SOME INPUT :)" << endl;
					cout << "Returning to the menu..." << endl;
					this_thread::sleep_for(std::chrono::seconds(3));

				}

				

			}

			if (counter == 4) {
				if (a.maxVertex > -1)
				{
					a.visualization();
					cout << "Returning to the menu..." << endl;
					this_thread::sleep_for(std::chrono::seconds(2));

				}
				else {
					system("cls");

					cout << "THERE IS NO GRAPH FOR THE INPUT, KINDLY GIVE US SOME INPUT :)" << endl;
					cout << "Returning to the menu..." << endl;
					this_thread::sleep_for(std::chrono::seconds(3));

				}
				
			}

			if (counter == 5) {
				
				exit(0);
			}


		}


	}

	return 0;

}

