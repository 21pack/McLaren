Вообще тут должны быть всякие алгоритмы и прочая математика, но пока что ничего.

Один кадр:
Engine::run()
 ├─ Input.pollEvents(Renderer) (Считывает клавиши и события SFML)
 │
 ├─ activeLoop.update(dt)
 │       └─ World.update(dt)
 │           └─ Entity.update(dt)
 │
 ├─ renderer.clear()
 ├─ activeLoop->draw(renderer, camera)
 │       └─ World.draw(renderer, camera)
 │           └─ Entity.render(renderer, camera)
 │               └─ renderer.draw(sprite)
 │
 ├─ renderer.present()
 └─ if (loop->isFinished()) -> exit()
