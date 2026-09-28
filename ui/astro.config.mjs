import { defineConfig } from 'astro/config';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const shouldMockApi = process.env.UI_STANDALONE_MOCK === 'true';

export default defineConfig({
  vite: {
    resolve: {
      alias: {
        // Alias the module used for backend api calls.
        // This allows using mock calls and data when running just the UI.
        '@api': shouldMockApi 
          ? path.resolve(__dirname, './src/api/mockApi.ts') 
          : path.resolve(__dirname, './src/api/api.ts')
      }
    }
  },
  experimental: {
    // Auto-prerenders back/forward.
    // Hopefully to make it snappier :) 
    clientPrerender: true, 
  }
});
