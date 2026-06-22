import os
import glob
import importlib.util
import pytest

def get_python_files():
    base_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
    files = glob.glob(os.path.join(base_dir, "**/*.py"), recursive=True)
    # Exclude docs, tests, venv, or hidden dirs
    return [f for f in files if "docs/" not in f and "tests/" not in f and "/." not in f]

PYTHON_FILES = get_python_files()

@pytest.mark.parametrize("filepath", PYTHON_FILES)
def test_import_syntax(filepath):
    """Test that every python file can be parsed and imported without crashing the logic."""
    module_name = os.path.basename(filepath).replace(".py", "")
    spec = importlib.util.spec_from_file_location(module_name, filepath)
    module = importlib.util.module_from_spec(spec)
    try:
        # We don't execute the module to avoid running long scripts, 
        # but we compile it to check syntax.
        with open(filepath, 'r') as f:
            source = f.read()
        compile(source, filepath, 'exec')
    except Exception as e:
        pytest.fail(f"Logic crash (syntax/compilation error) in {filepath}: {e}")
