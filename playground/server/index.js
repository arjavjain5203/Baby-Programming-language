const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(cors());
app.use(bodyParser.json());

const COMPILER_PATH = path.resolve(__dirname, '../../build/baby');
const TEMP_FILE = path.resolve(__dirname, 'temp.by');
const OUT_ASM = 'out.asm'; // Compiler generates this in CWD
const OUT_EXEC = './out'; // Compiler generates this in CWD

// Ensure we are working in the server directory so compiler outputs are contained
// Actually, the compiler generates outputs in CWD. So we should run exec with cwd options.

app.post('/compile', (req, res) => {
    const { code } = req.body;

    // Save code to temp file
    fs.writeFileSync(TEMP_FILE, code);

    // Run compiler
    // We run in the current directory so out.asm and out are generated here
    exec(`${COMPILER_PATH} ${TEMP_FILE}`, { cwd: __dirname }, (error, stdout, stderr) => {
        if (error || stderr) {
            // Compilation failed
            const errorMsg = stderr || error.message;
            // Try to make it sarcastic if it's a generic error, but relying on compiler's output is better
            return res.json({
                success: false,
                output: '',
                assembly: '',
                errors: errorMsg
            });
        }

        // Compilation success
        // Read assembly
        let assembly = '';
        try {
            assembly = fs.readFileSync(path.resolve(__dirname, OUT_ASM), 'utf-8');
        } catch (e) {
            assembly = '; Assembly file not found. Distinctly odd.';
        }

        // Run the executable
        exec(OUT_EXEC, { cwd: __dirname }, (runError, runStdout, runStderr) => {
            const output = runStdout + runStderr; // Capture both

            let runtimeError = null;
            if (runError) {
                runtimeError = runStderr || runError.message;
            }

            res.json({
                success: !runtimeError,
                output: output,
                assembly: assembly,
                errors: runtimeError ? `Runtime Error:\n${runtimeError}` : ''
            });

        });
    });
});

const PORT = 3001;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
