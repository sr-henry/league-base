# League Base

League Base is an external hack base for League of Legends, designed to utilize image processing techniques to identify in-game entities and expose essential game information. This project aims to provide a solid foundation for developers interested in creating hacks, mods, or custom tools for League of Legends.

## Disclaimer 
Before proceeding, it is crucial to acknowledge that using hacks or any form of cheating in online games, including League of Legends, violates the game's terms of service. This project is purely for educational and experimental purposes and should not be used in any live or competitive environment. The developers and contributors of League Base do not encourage or endorse cheating or unfair gameplay.

## Features
* **Image Processing**: League Base employs advanced image processing algorithms to capture and analyze the game's screen in real-time, allowing for the identification of various in-game entities such as champions, minions, monsters, and structures.

* **Exposed Game Information**: The base exposes relevant game information through an API or interface, making it easier for developers to access essential data. This includes player coordinates, champion health, mana, cooldowns, and other critical statistics.

* **Customization**: League Base is designed with flexibility in mind. Developers can easily extend and customize the base to suit their specific hacking needs and desired features.

* **External Approach**: This hack base operates externally to the game, minimizing the risk of detection and potential game bans. However, it is essential to remain cautious and avoid any behavior that could trigger anti-cheat mechanisms.

## Getting Started
To get started with League Base, follow these steps:

1. Clone the League Base repository to your local machine.

```bash
git clone https://github.com/sr-henry/league-base.git
```

2. Goto the dependencies folder opencv and create a reference in the project to the opencv_world452.dll file or move the dll to the system32 folder

3. Copy the game cfg files and replace the default ones.

4. Open de main file and start creating your hack.
```cpp
// Example
#include "league-base/base.h"

using namespace league_base;

int main()
{

	while (!GetAsyncKeyState(VK_HOME))
	{	
		if (::hack.IsGameRunning())
		{
			::hack.Update();

			// Features here...

		}
	}
	
	return 0;
}

```

## Limitations

While League Base aims to provide a robust foundation for external hack development in League of Legends, it comes with certain limitations that developers should be aware of. These limitations are primarily due to the nature of processing a 2D image of a 3D game world, which can result in discrepancies between screen positions and actual in-game world positions. Here are some important limitations to consider:

1. **Screen Resolution and Aspect Ratio:** League Base relies on capturing and analyzing the game's screen, which means that changes in the player's screen resolution or aspect ratio may impact the accuracy of entity detection. Different screen resolutions and aspect ratios can alter the positioning and scaling of in-game elements, affecting the image processing algorithms.

2. **Camera Zoom Levels:** League of Legends allows players to zoom in and out of the game world using the in-game camera. Varying camera zoom levels can result in different object sizes and positions on the screen, potentially leading to inconsistencies in entity identification.

3. **Perspective Distortions:** The 3D game world is projected onto a 2D screen, causing perspective distortions. As a result, objects that are farther away from the camera may appear smaller on the screen, potentially impacting the accuracy of image processing algorithms that rely on object size.

4. **Dynamic Game Elements:** League of Legends is a dynamic game with various animations, effects, and interactions. These dynamic elements can introduce complexities in accurately identifying and tracking entities, especially during fast-paced gameplay or during the presence of multiple simultaneous effects.

5. **Anti-Cheat Measures:** League of Legends employs anti-cheat mechanisms to detect and prevent hacking activities. While League Base operates externally to the game, it is essential to be cautious and avoid any behavior that may trigger anti-cheat detection.

## Mitigating Strategies

To mitigate the impact of these limitations, developers working with League Base should consider the following strategies:

- **Configuration Options:** Provide configuration options within the hack base that allow users to adjust settings like screen resolution, camera zoom levels, and image processing parameters based on their specific setup.

- **Testing Across Resolutions:** Thoroughly test the hack base across different screen resolutions and aspect ratios to identify potential issues and ensure broader compatibility.

- **Dynamic Element Handling:** Implement algorithms that can adapt to dynamic elements by utilizing motion detection, pattern recognition, or state tracking to improve entity identification accuracy.

- **Ethical Use:** Emphasize responsible and ethical use of the hack base, avoiding any malicious or harmful activities that could harm the integrity of the game or the experience of other players.

## Benchmark
| HW  | Setting | Usage |
|-----|---------|-------|
| CPU | Ryzen 5 3600 | 33.5% |
| GPU | RTX 3060 ti | 2%    |
| MEM | 8Gb | 23Mb |


DEMO
-----
[VIDEO](https://youtu.be/d71nUvfhn64)

![](demo.gif)
