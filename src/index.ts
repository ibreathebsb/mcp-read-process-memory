import { McpServer } from "@modelcontextprotocol/sdk/server/mcp.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio.js";
import { z } from "zod";
import { execFile } from "child_process";

// Create server instance
const server = new McpServer({
  name: "read-process-memory",
  version: "1.0.0",
  capabilities: {
    resources: {},
    tools: {},
  },
});

server.tool(
  "read-process-memory",
  "Read memory of specified process",
  {
    address: z
      .string()
      .regex(/^0[xX][0-9a-fA-F]+$/)
      .describe("The target memory address in HEXADECIMAL format"),
    type: z
      .enum(["uint8", "uint16", "uint32", "uint64"])
      .describe("The size of memory to read"),
    processId: z
      .number()
      .positive()
      .describe("The ID of the process to read memory from"),
  },
  async ({ address, type, processId }) => {
    const actualAddress = parseInt(address, 0);
    if (isNaN(actualAddress)) {
      return {
        content: [
          {
            type: "text",
            text: "address must be in HEXADECIMAL format with prefix 0x or 0X",
          },
        ],
      };
    }
    const data = await fetch(`http://127.0.0.1:20258/`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ address: actualAddress, type, processId }),
    });
    const res = await data.json();

    if (res && res.success) {
      return {
        content: [
          {
            type: "text",
            text: "0x" + res.data.toString(16),
          },
        ],
      };
    }
    return {
      content: [
        {
          type: "text",
          text: "failed to read memory",
        },
      ],
    };
  }
);

async function main() {
  const transport = new StdioServerTransport();
  await server.connect(transport);
}

const pmm = execFile(__dirname + "/PMM.exe", (error, stdout, stderr) => {
  if (error) {
    console.error("Error starting PMM:", error);
    process.exit(1);
  }
});

process.on("exit", () => {
  pmm.kill();
});

main().catch((error) => {
  console.error("Fatal error in main():", error);
  process.exit(1);
});
