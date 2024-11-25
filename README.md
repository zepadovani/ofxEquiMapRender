# ofxEquiMapRender

ofxEquiMapRender is an OpenFrameworks addon designed to render equirectangular projections efficiently. This addon is built upon the following projects:

* **[ofxEquiMap](https://github.com/hanasaan/ofxEquiMap)** by Yuya Hanai. For more details, see the [forum discussion](http://forum.openframeworks.cc/t/equirectangular-projection-shader/19937).
* **[ofxCubeMap](https://github.com/andreasmuller/ofxCubeMap)** by Andreas Muller. Note: This is *not* the same `ofxCubeMap` included in the OpenFrameworks GL library.

The addon was created with a distinct name to avoid confusion with the older libraries and the built-in `ofxCubeMap` distributed with OpenFrameworks. It integrates code from these projects and introduces new features compatible with OpenFrameworks 0.12 and later.

## Features

- Simple and efficient rendering of equirectangular images.
- Compatible with OpenFrameworks 0.12+.
- Integrates and extends functionality from ofxEquiMap and ofxCubeMap.

## Usage

Refer to the examples provided in the `examples` directory for usage instructions.

## Installation

1. Clone the repository into your OpenFrameworks `addons` directory:
    ```sh
    git clone https://github.com/yourusername/ofxEquiMapRender.git
    ```
2. Include the addon in your OpenFrameworks project.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

Special thanks to Yuya Hanai and Andreas Muller for their foundational work on ofxEquiMap and ofxCubeMap.
For any questions or contributions, feel free to open an issue or submit a pull request.

## ToDo: 
- [ ] Create single image for skyBox image (4:3 aspect ratio)
- [ ] Add a full dome mode (1:1 aspect ratio)
- [ ] Create a camera (to move the point of view accross a scene)

