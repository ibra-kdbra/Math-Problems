/// <reference lib="webworker" />

const ctx: Worker = self as any;

// Load Pyodide from CDN
importScripts('https://cdn.jsdelivr.net/pyodide/v0.25.0/full/pyodide.js');

let pyodideReadyPromise: Promise<any>;

async function loadPyodideAndPackages() {
  // @ts-ignore - loadPyodide is available globally after importScripts
  const pyodide = await loadPyodide();
  // Preload common scientific packages you might need
  // await pyodide.loadPackage(['numpy', 'matplotlib']);
  return pyodide;
}

pyodideReadyPromise = loadPyodideAndPackages();

self.onmessage = async (event: MessageEvent) => {
  const { id, python, ...context } = event.data;

  // The worker will respond to multiple events. If python is provided, execute it.
  if (!python) return;

  try {
    const pyodide = await pyodideReadyPromise;
    
    // Redirect stdout/stderr to capture print statements
    let output = '';
    pyodide.setStdout({ batched: (msg: string) => { output += msg + '\n'; } });
    pyodide.setStderr({ batched: (msg: string) => { output += msg + '\n'; } });

    // Execute the python code
    await pyodide.runPythonAsync(python);
    
    self.postMessage({ id, results: output, error: null });
  } catch (error: any) {
    self.postMessage({ id, results: null, error: error.message });
  }
};
