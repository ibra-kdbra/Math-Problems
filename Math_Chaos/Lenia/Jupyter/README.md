# Understanding Lenia Cellular Automata with Jupyter Notebook

## Introduction to Lenia

Lenia is a type of continuous cellular automata (CCA) developed by Bert Chan in 2019. Unlike traditional cellular automata, which use discrete states, Lenia uses continuous states to represent the state of each cell in the grid. This allows for more complex and intricate patterns to emerge, making Lenia a fascinating subject of study in the field of artificial life and complex systems.

## Exploring Lenia in Jupyter Notebook

In this tutorial, we'll use Jupyter Notebook to investigate the behavior of Lenia and visualize the results.

### Prerequisites

- Python 3.x
- Jupyter Notebook
- NumPy
- Matplotlib

### Step 1: Set up the Lenia environment

First, let's import the necessary libraries and define the Lenia function:

```python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def lenia(state, r, c, k, s, b):
    """
    Lenia cellular automata function.
    
    Parameters:
    state (numpy.ndarray): Current state of the grid.
    r (float): Interaction radius.
    c (float): Interaction center.
    k (float): Kernel amplitude.
    s (float): Kernel steepness.
    b (float): Birth threshold.
    
    Returns:
    numpy.ndarray: Updated state of the grid.
    """
    # Calculate the distance matrix
    dist = np.sqrt((np.arange(state.shape[0])[:, None] - r)**2 + (np.arange(state.shape[1])[None, :] - c)**2)
    
    # Apply the kernel function
    kernel = k * np.exp(-((dist - c) / s)**2)
    
    # Update the state
    new_state = state + (kernel > b) - (state > 0)
    
    return np.clip(new_state, 0, 1)
```

### Step 2: Set up the initial state and parameters

Now, let's define the initial state of the grid and the Lenia parameters:

```python
# Set up the initial state
width, height = 100, 100
initial_state = np.random.rand(width, height)

# Define the Lenia parameters
r = 10
c = 5
k = 1
s = 2
b = 0.5
```

### Step 3: Visualize the Lenia dynamics

We can use Matplotlib's animation functionality to visualize the evolution of the Lenia grid over time:

```python
# Create the figure and axis
fig, ax = plt.subplots(figsize=(10, 10))
ax.set_xlim(0, width)
ax.set_ylim(0, height)
ax.set_aspect('equal')
ax.set_xticks([])
ax.set_yticks([])

# Define the update function for the animation
def update(frame):
    global initial_state
    initial_state = lenia(initial_state, r, c, k, s, b)
    ax.clear()
    ax.imshow(initial_state, cmap='binary')
    return [ax]

# Create the animation
ani = FuncAnimation(fig, update, frames=100, interval=50, blit=True)

# Display the animation
plt.show()
```

This code will create an animation that shows the evolution of the Lenia grid over 100 frames, with a frame rate of 20 frames per second.

### Step 4: Explore different Lenia parameters

You can experiment with different Lenia parameters (r, c, k, s, b) to see how they affect the resulting patterns and dynamics. Try adjusting the values and observe the changes in the grid's behavior.
