TEXSRC=raytracer.cpp Scene.cpp Image.cpp vec3.cpp Camera.cpp Ray.cpp Light.cpp Material.cpp

raytrace:
	g++ -std=c++11 $(TEXSRC) -o raytracer

clean:
	rm raytracer
