export interface SearchResult {
  id: string;
  title: string;
  url: string;
  snippet: string;
}

export interface SearchResponse {
  query: string;
  page: number;
  limit: number;
  totalPages: number;
  totalResults: number;
  results: SearchResult[];
}
