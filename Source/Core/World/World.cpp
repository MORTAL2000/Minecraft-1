#include "World.h"

namespace Minecraft
{
	World::World()
	{
		// Generate all the chunks 

		p_Player = new Player;

		Logger::LogToConsole("World Generation Began");

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				GenerateChunk(&m_WorldChunks[i][j]);
			}
		}

		Logger::LogToConsole("World Generation Ended");
		Logger::LogToConsole("Chunk Mesh construction began");

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Timer timer("Mesh Construction!");

				m_WorldChunks[i][j].Construct(glm::vec3(i,1,j));
			}
		}

		Logger::LogToConsole("Chunk Mesh construction ended");
	}

	World::~World()
	{

	}

	void World::OnUpdate(GLFWwindow* window)
	{
		p_Player->OnUpdate();

		const float camera_speed = 1.0f;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Front, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Back, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Left, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Right, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Up, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Down, camera_speed);
	}

	void World::RenderWorld()
	{
		const int render_distance = 4;

		std::vector<std::pair<int, int>> chunks;
		
		int player_chunk_x = 0;
		int player_chunk_y = 0;
		int player_chunk_z = 0;

		player_chunk_x = floor(p_Player->p_Position.x / ChunkSizeX);
		player_chunk_y = floor(p_Player->p_Position.y / ChunkSizeY);
		player_chunk_z = floor(p_Player->p_Position.z / ChunkSizeZ);

		RenderChunkFromMap(player_chunk_x, player_chunk_z);
		RenderChunkFromMap(player_chunk_x + 1, player_chunk_z);
		RenderChunkFromMap(player_chunk_x, player_chunk_z + 1);

		RenderChunkFromMap(player_chunk_x - 1, player_chunk_z);
		RenderChunkFromMap(player_chunk_x - 1, player_chunk_z + 1);
		RenderChunkFromMap(player_chunk_x, player_chunk_z - 1);
		RenderChunkFromMap(player_chunk_x + 1, player_chunk_z + 1);
		RenderChunkFromMap(player_chunk_x + 1, player_chunk_z - 1);
		RenderChunkFromMap(player_chunk_x - 1, player_chunk_z - 1);
		RenderChunkFromMap(player_chunk_x - 1, player_chunk_z + 1);
	}

	void World::OnEvent(EventSystem::Event e)
	{
		if (e.type == EventSystem::EventTypes::MouseMove)
		{
			p_Player->p_Camera.UpdateOnMouseMovement(e.mx, e.my);
		}
	}

	void World::RenderChunkFromMap(int cx, int cz)
	{
		std::map<int, std::map<int, Chunk>>::iterator chunk_exists_x = m_WorldChunks.find(cx);

		if (chunk_exists_x != m_WorldChunks.end())
		{
			std::map<int, Chunk>::iterator chunk_exists_z = m_WorldChunks.at(cx).find(cz);

			if (chunk_exists_z != m_WorldChunks.at(cx).end())
			{
				// The chunk exists. Render the already generated chunk
				m_Renderer.RenderChunk(&m_WorldChunks[cx][cz], &p_Player->p_Camera);

				return;
			}

			else
			{
				GenerateChunk(&m_WorldChunks[cx][cz]);
				m_WorldChunks[cx][cz].Construct(glm::vec3(cx,1,cz));
			}
		}

		else
		{
			GenerateChunk(&m_WorldChunks[cx][cz]);
			m_WorldChunks[cx][cz].Construct(glm::vec3(cx, 1, cz));
		}

		m_Renderer.RenderChunk(&m_WorldChunks[cx][cz], &p_Player->p_Camera);
		return;
	}
}