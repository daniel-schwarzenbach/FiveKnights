# Used Programming Concepts

## Generic Programming

Used by Matrix, Vec2, Vector, and Geometry.

See: `Senpai/include/core/container`

## Lambda Functions

Used for callbacks, as scene load functions, in the thread wrapper (`Senpai/core/Thread.h++`), and by the Chess Computer for parallelization.

## Inheritance

Events, Scripts, Assets, Components, and Systems are all based on inheritance.

Intermediate types such as UIComponent and RenderComponent are used to differentiate how they are rendered.

## Concepts and Type Traits

Events, Systems, Components, Scripts, and Assets make extensive use of concepts so that we can securely assign unique IDs to every subtype.

## Pseudo Dynamic Typing

Pseudo dynamic typing is not actively used by the game, but it would allow us to copy Entities, Components, and Systems without knowing all of their subtypes.

See: `Senpai/include/core/scene/ECSBase.h++`
- ComponentTypeRegistry
- SystemTypeRegistry

## Parallelization

The Chess Computer (`Game/src/ChessComputer.h++`) runs in parallel, and the newly loaded scene is also executed in parallel (`Senpai/src/Senpai.c++`).