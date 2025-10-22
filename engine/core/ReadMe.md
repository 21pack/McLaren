Вообще тут должны быть всякие алгоритмы и прочая математика, но пока что ничего.

Один кадр:
Engine::run()
 ├─ Input.pollEvents(Render) (Считывает клавиши и события SFML)
 │
 ├─ activeLoop.update(dt)
 │       └─ World.update(dt)
 │           └─ Entity.update(dt)
 │
 ├─ render.clear()
 ├─ activeLoop->draw(render, camera)
 │       └─ World.draw(render, camera)
 │           └─ Entity.render(render, camera)
 │               └─ render.draw(sprite)
 │
 ├─ render.present()
 └─ if (loop->isFinished()) -> exit()
