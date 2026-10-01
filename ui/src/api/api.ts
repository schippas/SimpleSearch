import type { SearchResponse } from './types';

export async function fetchSearchResults(query: string, page: number = 1): Promise<SearchResponse> {
  const apiUrl = import.meta.env.BACKEND_API_URL || 'http://localhost:8080';
  const response = await fetch(`${apiUrl}/api/v1/search?query=${encodeURIComponent(query)}&page=${page}`);

  if (!response.ok) {
    throw new Error(`Search request failed with status: ${response.status}`);
  }

  return await response.json();
}
