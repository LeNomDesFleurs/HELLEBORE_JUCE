# Hellebore Journal

## To-Do

- [ ] ~~Refactor Outils as an object with sample rate embedded ?~~
- [x] ~~Functional RingBuffer (all modes repitching)~~
- [x] ~~Functional Comb~~
- [x] ~~Functional AllPass~~ 
- [x] ~~Add freeze to ring buffer~~
- [x] ~~refactor filter.h~~
- [x] ~~functional reverb~~
- [x] ~~un peu bizarre de finir pas le allpass, il devrait plutôt être au début~~
- [x] ~~faire un fader qui fait bouger un cercle sur l'ui~~
- [ ] Faire un nouveau gradient de couleur
- [ ] getting rid of the security padding in RingBuffer.cpp ln:77
- [ ] utiliser des gradients radiaux transparent au bord pour masquer un motif background
- [ ] ajouter le logo
- [ ] find a way to not updateStepSize once it is stable
- [ ] Big problem, Ringbuffer s'instancie avant de recevoir son sample rate
- [ ] try stepped repitching ?
- [ ] adding modulation ?
- [ ] problème dans mon pan : je somme des potentiel problème de phase, quoi que ça doit pas être très problématique puisque c'est la sortie des combs
- [ ] augmenter taille max des buffers
- [ ] Gros problème de variation de volume, test à faire
- [x] ~~Se pencher sur les histoire de edit pendant que ça run ?~~

## Segmentation fault / Assertion failure

Le problème venait de l'initialisation de l'apvts, pas le bon nombre de paramètre, ça remplissait pas comme il faut.

## Symbol duplicate / symbol not found

Need to use `.hpp` and `.cpp` right, declaration in `.hpp`, definition in `.cpp`.
Also the problem was rendered more weird by some functions being inlined and some not.

## Gotta test ring buffer 😢

Ok let's get the ringbuffer file out of the backup and plug a parameter in this mess
Let's take one channel and test the delay line
Can't remember where I was at with this ring buffer thingy, I gotta check the function all have definition first.

The slew of the setstep should probably be done in the delay line, so it knows exactly the actual delay

Everything is refactored, for now it is not working 🤔

audio arrive, but there seems to be some kind od remanence of the past, and maybe some bit compression...
C'est très curieux y'a l'air d'y avoir une forme de feedback comme si le write écrivait pas des zero mais un mélange

essayer de remplacer la fonction increment read ?
(attentino j'ai change la valeur de retour de noInterpolation)

Le son arrive bien comme il faut, le problème vient pas de blkhole (puisque l'enceinte droite marche bien).

Refait une propos hyper basique avec un vecteur de donnée directe dans les plugin processor, exactement le même problème, je suppose que ça doit être inéhrent au vecteur...

J'AI ENVIE DE CREVERRRR 💀
Le problème c'est que je passais deux fois dans la boucle du plugin processor 😭

va falloir trouver un truc qui marche mieux pour repitch...

## freeze repitch

Pour le moment quand write continue à avancer, en freeze ça continue juste à tourner sur tout le buffer -> donc tout les buffers font la même taille d'un coup, pas top.
c'est bon, j'ai refactor avec plein de fonction spécial freeze

## update UI

En plusieurs points :

1. D'abord on ajoute le `public juce::AudioProcessorParameter::Listener` à l'éditeur
2. ça nous permet d'override `void parameterValueChanged(int parameterIndex, float newvalue) override;`
3. On ajoute `public juce::Timer`
4. ça nous permet d'override `void timerCallback() override;`
5. On le démarre avec `startTimerHz(60)` dans le constructeur de l'éditeur
6. On ajoute une variable `juce::atomic<bool> parametersChanged`

Le concept c'est qu'à chaque fois que la fonction listener est called, on change la variable à true, et le timer vérifie régulièrement si ça à été changé ou pas.

Pour que tout le monde soit bien listené, dans le constructeur de l'éditeur on ajoute

```cpp
const auto& params = audioProcessor.getParameters();
for(auto param : params){
    param->addListener(this);
}
```

Et on oublie pas de les enlever dans le déstructeur avec `param->removeListener`

### A

https://colinchcpp.github.io/2023-10-03/17-49-00-956504-implementing-audio-effects-using-comb-filters-in-c/

J'ai l'impression que le freeze mode casse le feedback...

## UI

How to Work with toggle knob : j'avais lu quelque part qu'il fallait éditer le paint du toggle, untoggle et coche
Allons chercher toggle dans le gros look & feel de JUCE
La fonction `drawToggleButton()` appelle la fonction `drawTickBox()` en lui passant son statut
essayont de faire un carré dans ce `drawToggle`
Pour faire des formes simple, [ce tutoriel de JUCE](https://docs.juce.com/master/tutorial_graphics_class.html)

```cpp
    Path roof;
    roof.addTriangle (0, 0, 10, 10, 10, 5);
    g.fillPath (roof);
    juce::Rectangle<int> house (0, 0, 10, 10);
    g.fillRect(house);
```

apparement moyen de faire un rounded rectangle

### curved slider

Pas mal de ressource dans [la doc de la classe Path](https://docs.juce.com/master/classPath.html#a6cfdf1dea900089ffee8eea61385062e)
Pour faire une courbe quadratic, on peut ajouter un point de control avec `.quadraticTo` au lieu du `.lineTo` qu'il y avait avant. Par contre faut bien définir les bounds du slider sinon ça sort du component et ça crop, donc faudra de l'ajustement pour que ce soit tout joli, faut que je dig les histoire 

## Définir les couleurs

J'ai ajouter un namespace dans le editor.h en déclarant les couleurs en `const` pour éviter des problèmes de header

```cpp
namespace CustomColors {
const juce::Colour brown = juce::Colour::fromString("FF50322a");
const juce::Colour green = juce::Colour::fromString("FF608537");
}
```

`fromString` parse le texte en Hexa, ne pas oublier le *FF* pour que la couleur soit opaque  

## Gradient de couleur

for future use :

```cpp
ColourGradient::ColourGradient(Colour colour1, float x1, float y1, Colout colour2, float x2, float y2, bool isRadio);
```

If isRadial is true, the colours form a circular gradient with (x1, y1) at its centre.

Aucune idée de la manière de l'utiliser concrètement parcontre

info on doing a conical gradient on [this thread](https://forum.juce.com/t/how-to-draw-conic-gradient/16070/13)

Sinon pour juste avoir une interpolation entre deux couleurs

```cpp
juce::Colour juce::Colour::interpolatedWith(juce::Colour, float proportion of other)
```

## Background notes

1. add the background component file
2. add an BackgroundComponent object to the editor
3. `addAndMakeVisible(background_component)` dans le constructeur de l'éditeur 
   **⚠️ avant les autres pour pas qu'il passe dessus ⚠️**
4. `paintOverChildren()` au lieu de `paint()`
5. `background_component.setBounds(getBounds)` dans `resize()`
6. **Ne pas appeller `background_component.paint()` dans `paintOverChildren()`**

## Size Widget

circle in rouded rectangle,
fill a placeholder rounder rectangle to multiply the circle
it'll avoid the corner

`Drawable.setClipPath()` ?

```cpp
Path vignette;
  vignette.addRoundedRectangle(0, 0, width, height, 5);

g.reduceClipRegion(vignette);
```

## Getting GLSL to work

[version problem](https://forum.juce.com/t/compiling-shaders/60354)
[get log](https://forum.juce.com/t/just-a-quick-gl-info-logger-func-for-any-of-you/32082)
[maybe useful](https://forum.juce.com/t/creating-an-opengl-3-2-context-easy-fix-included/48872)
[boilerplate that won't work](https://medium.com/@Im_Jimmi/using-opengl-for-2d-graphics-in-a-juce-plug-in-24aa82f634ff)
[multiple demos](https://forum.juce.com/t/an-demo-of-how-use-opengl-with-juce-anyone-want-to-do-it-together/35422)

Got it working with the Juce glsl 2D method, now I need to feed it a u_time

Might as well use CPU blur such as [melatonin Blur](https://github.com/sudara/melatonin_blur?tab=readme-ov-file)
