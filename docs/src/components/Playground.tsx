import React, { useState, useEffect, useRef } from 'react';

// Vite worker import syntax
import PyodideWorker from './pyodide.worker?worker';

export default function Playground({ initialCode = 'print("Hello from Math Problems!")' }: { initialCode?: string }) {
  const [code, setCode] = useState(initialCode);
  const [output, setOutput] = useState<string>('');
  const [isRunning, setIsRunning] = useState(false);
  const workerRef = useRef<Worker | null>(null);

  useEffect(() => {
    // Initialize the worker on component mount
    workerRef.current = new PyodideWorker();
    
    // Listen for messages from the worker
    workerRef.current.onmessage = (event) => {
      const { results, error } = event.data;
      if (error) {
        setOutput((prev) => prev + `\nError: ${error}`);
      } else if (results) {
        setOutput((prev) => prev + `\n${results}`);
      }
      setIsRunning(false);
    };

    return () => {
      workerRef.current?.terminate();
    };
  }, []);

  const handleRun = () => {
    if (!workerRef.current || isRunning) return;
    
    setIsRunning(true);
    setOutput('> Running...');
    // Send the code to the worker
    workerRef.current.postMessage({ id: Date.now(), python: code });
  };

  return (
    <div className="flex flex-col md:flex-row gap-4 my-8 rounded-xl overflow-hidden border border-border bg-background shadow-sm h-[500px]">
      {/* Editor Pane */}
      <div className="flex-1 flex flex-col border-r border-border">
        <div className="flex items-center justify-between px-4 py-2 bg-muted/50 border-b border-border">
          <span className="text-xs font-semibold text-muted-foreground uppercase tracking-wider">Python Editor</span>
          <button 
            onClick={handleRun}
            disabled={isRunning}
            className={`px-3 py-1 rounded text-xs font-medium transition-colors ${isRunning ? 'bg-primary/50 cursor-not-allowed' : 'bg-primary text-primary-foreground hover:bg-primary/90'}`}
          >
            {isRunning ? 'Executing...' : 'Run Code'}
          </button>
        </div>
        <textarea
          value={code}
          onChange={(e) => setCode(e.target.value)}
          className="flex-1 p-4 bg-transparent resize-none outline-none font-mono text-sm text-foreground"
          spellCheck="false"
        />
      </div>

      {/* Output Pane */}
      <div className="flex-1 flex flex-col bg-zinc-950">
        <div className="px-4 py-2 border-b border-zinc-800 bg-zinc-900">
          <span className="text-xs font-semibold text-zinc-400 uppercase tracking-wider">Terminal Output</span>
        </div>
        <pre className="flex-1 p-4 overflow-auto font-mono text-xs text-green-400 whitespace-pre-wrap m-0">
          {output || 'Ready.'}
        </pre>
      </div>
    </div>
  );
}
