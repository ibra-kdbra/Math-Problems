export const ColoringMandlebrotSamples = Array.from({ length: 2 }, (_, i) => ({
  src: `/imgs/mandlebrot_coloring/sample_${i}.png`,
  alt: `Sample ${i}`
}))

export const buddhabrotSamples = Array.from({ length: 4 }, (_, i) => ({
  src: `/imgs/buddhabrot_computation/sample_${i}.jpg`,
  alt: `Sample ${i}`
}));

export const fractalghSamples = Array.from({ length: 9 }, (_, i) => ({
  src: `/imgs/fractal_gh/fractal-${i}.jpg`,
  alt: `fractal ${i}`
}));

export const sortingSamples = Array.from({ length: 6 }, (_, i) => ({
  src: `/imgs/sorting_algorithms/${i + 1}.gif`,
  alt: `Algorithm ${i + 1} Visualization`
}));

export const pathPlanningImages = [
  { src: '/imgs/path_planning/bi_rrt.png', alt: 'Bi-RRT Path Planning Visualization' },
  { src: '/imgs/path_planning/apf.png', alt: 'Artificial Potential Field Visualization' }
];

export const quineMcCluskeyImages = Array.from({ length: 3 }, (_, i) => ({
  src: `/imgs/quine_mccluskey/Sample${i + 1}.jpg`,
  alt: `Sample Run ${i + 1}`
}));

export const mandelbrotExplorerImages = [
  { src: '/imgs/mandlebrot_explorer/image.png', alt: 'Mandelbrot Set Explorer Sample' }
];

export const lorenzImages = Array.from({ length: 3 }, (_, i) => ({
  src: `/imgs/lorenz_attractor/screenshot${i + 1}.png`,
  alt: `Lorenz Attractor Visualization ${i + 1}`
}));
