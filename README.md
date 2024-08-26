# raylib-rpg

My effort at making an RPG using raylib and C++.

- [www.evildojo.com](https://www.evildojo.com)
- [x.com/evildojo666](https://www.x.com/evildojo666)
- [youtube.com/@evildojo666](https://www.youtube.com/@evildojo666)
- [ko-fi.com/evildojo666](https://ko-fi.com/evildojo666)

-----

## origin

**Note: As of August 25, 2024, this work is considered in EXTREME ALPHA. No guarantees on the current state of the game! Build and run at your own risk!**

In 2013, I began working on a game I titled [Project A](https://github.com/mikedesu/project-a-ios) that was being written in Objective-C using cocos2d. The basis of graphics was centered around a `CCMutableTexture` hack that allowed me to define graphics in code. I began working on a dungeon crawler that was generated entirely in code. This was a very fun project but was only funded for 10 months, and I was still very, very new at this whole big tech and software industry. I had previously used cocos2d to make a "meta-game" that wasn't very good but worked as a proof-of-concept. 

Flash forward to mid-2014. I'm working as a corporate iOS developer and I discover [lazyfoo SDL2 tutorials in C++](https://lazyfoo.net/tutorials/SDL/index.php). I was also playing with the same ideas in Java using the [Eclispe HCR](https://wiki.eclipse.org/FAQ_What_is_hot_code_replace%3F) technique. Unfortunately I did not get very far into this before life interrupted.

Enter long drawn-out side-quest into security and [bug bounty](https://bugcrowd.com/darkmagex6) (like 2016-2024).

A few years ago, I participated in a gamedev contest on EFNet that led to the birth of [ecs](https://github.com/mikedesu/ecs), which is entirely in C++ and SDL2, and skull.exe, a side-scrolling shootemup. 

However, I recently discovered [raylib](https://github.com/raysan5/raylib/) and have fallen in love hard!

ECS and skull.exe are on indefinite hiatus in favor of this RPG engine. 

Out of the developments here, I hope to give birth to something that many people will enjoy and appreciate.

Wish me luck, and check on my progress every so often! Thank you for reading. Love and peace!

-----

## building

```
git clone https://github.com/mikedesu/raylib-rpg
cd raylib-rpg
make
```

-----

## running

While still in the `raylib-rpg` folder, you need to make sure you have the current assets. 
The following commands will:

1. download the `projectrpg-img.zip` file
2. unzip the file to reveal an `img` directory as well as several `*_textures.txt` files.
3. run the game

```
wget https://www.evildojo.com/projectrpg-img.zip
unzip projectrpg-img.zip
./game
```

Of course, if you've already grabbed the assets, you can just:

```
./game
```

-----

