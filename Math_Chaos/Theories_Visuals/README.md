
# Chaos Theory

## Graphs and computes systems in Chaos Theory; Lorenz Attractors & Logistic Maps

Visualize chaos with your own chosen parameters.

<p align="center">

[3D Lorenz Attractor](https://github.com/DorsaRoh/Chaos-Theory/blob/main/3D_lorenzattractor.py) : 3D representation of the Lorenz Attractor

<img width="800px" src="assets\3d_lorenz.png">

[2D Lorenz Attractor](https://github.com/DorsaRoh/Chaos-Theory/blob/main/2D_lorenzattractor.py) : 2D representation of the Lorenz Attractor's axes and dimension

<img width="850px" src="assets\2d_lorenz.png">

[Logistic Map](https://github.com/DorsaRoh/Chaos-Theory/blob/main/logisticmap.py) : 2D Representation of Logistic Map

<img width="600px" src="assets\LogisticMap.png">
</p>

## Features

- Computes and graphs Lorenz Attractors using values sigma, beta, Rho and time inputted by the user.
- Trajectory curve graphs, XY, YZ and XZ axis matplotlib graph visuals.
- Logistic Map using an inputted value for the r axis. Bifurcation can be controlled by the number of iterations given by the user.

### 1. Install dependencies

```shell
pip install numpy matplotlib scipy
```

or

```shell
pip install -r requirements.txt
```

### 3. Usage

**Enter your parameters in [parameters.py](/Math_Chaos/Chaos-Theory_Visuals/parameters.py) file**

Run file

```shell
python run *file name of desired chaotic system*
```

For example:

```shell
python 3D_lorenzattractor.py
```
